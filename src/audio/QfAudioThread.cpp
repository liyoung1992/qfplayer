#include "QfAudioThread.h"

QfAudioThread::QfAudioThread()
{
	if (!resample) 
		resample = new QfResample();
	if (!audio_play) 
		audio_play = QfAudioPlay::instance();
}

QfAudioThread::~QfAudioThread()
{
	is_exit = true;
	wait();
}

bool QfAudioThread::open(AVCodecParameters *para, int sampleRate, int channels)
{
	if (!para)
		return false;
	clear();
	audio_mutex.lock();
// 	if (!decode) 
// 		decode = new QFDecode();
// 	if (!resample)
// 		resample = new QfResample();
// 	if (!audio_play) 
// 		audio_play = QfAudioPlay::instance();
	pts = 0;
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
	audio_mutex.unlock();
	std::cout << "XAudioThread::Open :" << re << std::endl;
	return re;
}

void QfAudioThread::close()
{
	QfDecodeThread::close();
	if (resample) {
		resample->close();
		audio_mutex.lock();
		delete resample;
		resample = NULL;
		audio_mutex.unlock();
	}
	if (audio_play) {
		audio_play->close();
		audio_mutex.lock();
		audio_play = NULL;
		audio_mutex.unlock();
	}
}

// void QfAudioThread::push(AVPacket* pkt)
// {
// 	if (!pkt)return;
// 	//阻塞
// 	while (!is_exit)
// 	{
// 		audio_mutex.lock();
// 		if (packs.size() < max_list)
// 		{
// 			packs.push_back(pkt);
// 			audio_mutex.unlock();
// 			break;
// 		}
// 		audio_mutex.unlock();
// 		msleep(1);
// 	}
// }

void QfAudioThread::run()
{
	unsigned char *pcm = new unsigned char[1024 * 1024 * 10];
	while (!is_exit)
	{
		audio_mutex.lock();
		if (this->is_pause)
		{
			audio_mutex.unlock();
			msleep(5);
			continue;
		}
		//没有数据
// 		if (packs.empty() || !decode || !resample || !audio_play)
// 		{
// 			audio_mutex.unlock();
// 			msleep(1);
// 			continue;
// 		}
		AVPacket *pkt = pop();
// 		AVPacket *pkt = packs.front();
// 		packs.pop_front();
		bool re = decode->send(pkt);
		if (!re)
		{
			audio_mutex.unlock();
			msleep(1);
			continue;
		}
		//一次send 多次recv
		while (!is_exit)
		{
			AVFrame * frame = decode->recv();
			if (!frame) break;
			//减去缓冲未播放的时间
			pts = decode->get_pts() - audio_play->get_remainder_ms();
			//重采样 
			int size = resample->resample(frame, pcm);
			//播放音频
			while (!is_exit)
			{
				if (size <= 0)break;
				//缓冲未播完，空间不够
				if (audio_play->getFree() < size ||  is_pause)
				{
					msleep(1);
					continue;
				}
				audio_play->write(pcm, size);
				break;
			}
		}
		audio_mutex.unlock();
	}
	delete pcm;
}

void QfAudioThread::clear()
{
	QfDecodeThread::clear();
	mux.lock();
	if (audio_play)
		audio_play->clear();
	mux.unlock();
}

long long QfAudioThread::get_pts() const
{
	return pts;
}

void QfAudioThread::set_pause(const bool& pause)
{
	is_pause = pause;
	if (audio_play)
		audio_play->set_pause(pause);
}

bool QfAudioThread::get_pause() const
{
	return is_pause;
}
