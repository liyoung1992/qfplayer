#include "QfDemuxThread.h"
using namespace std;

void QfDemuxThread::run()
{
	while (!is_exit)
	{
		mux.lock();
		//暂停
		if (is_pause) {
			mux.unlock();
			msleep(6);
			continue;
		}
	
		if (!demux)
		{
			mux.unlock();
			msleep(5);
			continue;
		}

		//音视频同步
		if (video_thread && audio_thread)
		{
			pts = audio_thread->get_pts();
			video_thread->set_synpts(audio_thread->get_pts());
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
	totalMs = demux->get_total_ms();
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

void QfDemuxThread::close()
{
	is_exit = true;
	wait();
	if (video_thread)
		video_thread->close();
	if (audio_thread)
		audio_thread->close();
	mux.lock();
	delete video_thread;
	delete audio_thread;
	video_thread = NULL;
	audio_thread = NULL;
	mux.unlock();
}

void QfDemuxThread::clear()
{
	mux.lock();
	if (demux)
		demux->clear();
	if (video_thread)
		video_thread->clear();
	if (audio_thread)
		audio_thread->clear();
	mux.unlock();
}

void QfDemuxThread::seek(const double& pos)
{

	//清理
	clear();
	//暂停
	mux.lock();
	bool cur_status = is_pause;
	mux.unlock();

	set_pause(true);
	mux.lock();
	if (demux)
		demux->seek(pos);
	//实际显示的位置
	long long seekPts = pos * demux->get_total_ms();
	while (!is_exit)
	{
		AVPacket *pkt = demux->read_video();
		if (!pkt) break;
		//如果解码到seekPts
		if (video_thread->repaint_to_pts(pkt, seekPts))
		{
			this->pts = seekPts;
			break;
		}
	}

	mux.unlock();
	if (!cur_status) {
		set_pause(false);
	}
}

QfDemuxThread::~QfDemuxThread()
{
	is_exit = true;
	wait();
}

long long QfDemuxThread::get_pts() const
{
	return pts;
}

long long QfDemuxThread::get_total_ms() const
{
	return totalMs;
}



void QfDemuxThread::set_pause(const bool& pause)
{
	mux.lock();
	is_pause = pause;
	if (video_thread)
		video_thread->set_pause(pause);
	if (audio_thread)
		audio_thread->set_pause(pause);
	mux.unlock();
}

bool QfDemuxThread::get_pause() const
{
	return is_pause;
}
