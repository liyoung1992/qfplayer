#include "QfAudioThread.h"

QfAudioThread::~QfAudioThread()
{

}

bool QfAudioThread::open(AVCodecParameters *para, int sampleRate, int channels)
{
	if (!para)
		return false;
	mutex.lock();
	if (!decode) 
		decode = new QFDecode();
	if (!resample)
		resample = new QfResample();
	if (!audio_play) 
		audio_play = QfAudioPlay::instance();
	bool re = true;
	if (!resample->open(para, false))
	{
		std::cout << "Qfresampleample open failed!" << std::endl;
		re = false;
	}
	audio_play->sampleRate = sampleRate;
	audio_play->channels = channels;
	if (!audio_play->open())
	{
		re = false;
		std::cout << "QfAudioPlay open failed!" << std::endl;
	}
	if (!decode->open(para))
	{
		std::cout << "audio QFDecode open failed!" << std::endl;
		re = false;
	}
	mutex.unlock();
	std::cout << "XAudioThread::Open :" << re << std::endl;
	return re;
}

void QfAudioThread::push(AVPacket* pkt)
{
	if (!pkt)return;
	//阻塞
	while (!is_exit)
	{
		mutex.lock();
		if (packs.size() < max_list)
		{
			packs.push_back(pkt);
			mutex.unlock();
			break;
		}
		mutex.unlock();
		msleep(1);
	}
}

void QfAudioThread::run()
{
	unsigned char *pcm = new unsigned char[1024 * 1024 * 10];
	while (!is_exit)
	{
		mutex.lock();

		//没有数据
		if (packs.empty() || !decode || !resample || !audio_play)
		{
			mutex.unlock();
			msleep(1);
			continue;
		}

		AVPacket *pkt = packs.front();
		packs.pop_front();
		bool re = decode->send(pkt);
		if (!re)
		{
			mutex.unlock();
			msleep(1);
			continue;
		}
		//一次send 多次recv
		while (!is_exit)
		{
			AVFrame * frame = decode->recv();
			if (!frame) break;
			//重采样 
			int size = resample->resample(frame, pcm);
			//播放音频
			while (!is_exit)
			{
				if (size <= 0)break;
				//缓冲未播完，空间不够
				if (audio_play->getFree() < size)
				{
					msleep(1);
					continue;
				}
				audio_play->write(pcm, size);
				break;
			}
		}
		mutex.unlock();
	}
	delete pcm;
}
