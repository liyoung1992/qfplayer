#include "QfDemuxThread.h"
using namespace std;

void QfDemuxThread::run()
{
	while (!is_exit)
	{
		mux.lock();
		if (!demux)
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		AVPacket *pkt = demux->read();
		if (!pkt)
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		//����Ƶͬ��
		if (video_thread && audio_thread) {
			video_thread->set_synpts(audio_thread->get_pts());
		}
		//�ж���������Ƶ
		if (demux->isAudio(pkt))
		{
			if (audio_thread)
				audio_thread->push(pkt);
		}
		else //��Ƶ
		{
			if (video_thread)
				video_thread->push(pkt);
		}
		mux.unlock();
		msleep(1);
	}
}


bool QfDemuxThread::open(const char *url, QFVideoCallback *call)
{
	if (url == 0 || url[0] == '\0')
		return false;

	mux.lock();
	if (!demux) 
		demux = new QFDemux();
	if (!video_thread) 
		video_thread = new QfVideoThread();
	if (!audio_thread) 
		audio_thread = new QfAudioThread();

	//�򿪽��װ
	bool re = demux->open(url);
	if (!re)
	{
		cout << "demux->Open(url) failed!" << endl;
		return false;
	}
	//����Ƶ�������ʹ����߳�
	if (!video_thread->open(demux->copyVideoPara(),
		call, demux->width(), demux->height()))
	{
		re = false;
		cout << "video_thread->Open failed!" << endl;
	}
	//����Ƶ�������ʹ����߳�
	if (!audio_thread->open(demux->copyAudioPara(),
		demux->sample_rate(), demux->channels()))
	{
		re = false;
		cout << "audio_thread->Open failed!" << endl;
	}
	mux.unlock();
	//cout << "QfDemuxThread::Open " << re << endl;
	return re;
}
//���������߳�
void QfDemuxThread::start()
{
	mux.lock();
	if (!demux) 
		demux = new QFDemux();
	if (!video_thread) 
		video_thread = new QfVideoThread();
	if (!audio_thread) 
		audio_thread = new QfAudioThread();
	//������ǰ�߳�
	QThread::start();
	if (video_thread)
		video_thread->start();
	if (audio_thread)
		audio_thread->start();
	mux.unlock();
}

QfDemuxThread::~QfDemuxThread()
{
	is_exit = true;
	wait();
}
