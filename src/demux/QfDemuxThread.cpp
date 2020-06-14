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
		//音视频同步
		if (video_thread && audio_thread) {
			video_thread->set_synpts(audio_thread->get_pts());
		}
		//判断数据是音频
		if (demux->isAudio(pkt))
		{
			if (audio_thread)
				audio_thread->push(pkt);
		}
		else //视频
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

	//打开解封装
	bool re = demux->open(url);
	if (!re)
	{
		cout << "demux->Open(url) failed!" << endl;
		return false;
	}
	//打开视频解码器和处理线程
	if (!video_thread->open(demux->copyVideoPara(),
		call, demux->width(), demux->height()))
	{
		re = false;
		cout << "video_thread->Open failed!" << endl;
	}
	//打开音频解码器和处理线程
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
//启动所有线程
void QfDemuxThread::start()
{
	mux.lock();
	if (!demux) 
		demux = new QFDemux();
	if (!video_thread) 
		video_thread = new QfVideoThread();
	if (!audio_thread) 
		audio_thread = new QfAudioThread();
	//启动当前线程
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
