#include "QfDemuxThread.h"
using namespace std;

void QfDemuxThread::run()
{
	while (!is_exit)
	{
		mux.lock();
		//��ͣ
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

		//����Ƶͬ��
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
	totalMs = demux->get_total_ms();
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

	//����
	clear();
	//��ͣ
	mux.lock();
	bool cur_status = is_pause;
	mux.unlock();

	set_pause(true);
	mux.lock();
	if (demux)
		demux->seek(pos);
	//ʵ����ʾ��λ��
	long long seekPts = pos * demux->get_total_ms();
	while (!is_exit)
	{
		AVPacket *pkt = demux->read_video();
		if (!pkt) break;
		//������뵽seekPts
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
