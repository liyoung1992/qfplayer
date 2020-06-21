#pragma once
#include <qthread.h>
#include <mutex>
#include "../video/QfVideoCallback.h"
#include "../audio/QfAudioThread.h"
#include "../video/QfVideoThread.h"
#include "QfDemux.h"
class QfDemuxThread : public QThread
{
public:
	//创建对象并打开
	virtual bool open(const char *url, QFVideoCallback *call);

	//启动所有线程
	virtual void start();
	virtual void close();
	virtual void clear();
	virtual void seek(const double& pos);

	void run();
	QfDemuxThread() = default;
	virtual ~QfDemuxThread();

	long long get_pts() const;
	long long get_total_ms() const;

	void set_pause(const bool& pause);
	bool get_pause() const;

private:
	bool is_exit = false;
	std::mutex mux;
	QFDemux *demux = 0;
	QfVideoThread *video_thread = NULL;
	QfAudioThread *audio_thread = NULL;
	long long pts = 0;
	long long totalMs = 0;
	//bool running = false;
	bool is_pause = false;
};

