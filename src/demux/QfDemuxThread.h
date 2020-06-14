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
	//�������󲢴�
	virtual bool open(const char *url, QFVideoCallback *call);

	//���������߳�
	virtual void start();

	void run();
	QfDemuxThread() = default;
	virtual ~QfDemuxThread();

private:
	bool is_exit = false;
	std::mutex mux;
	QFDemux *demux = 0;
	QfVideoThread *video_thread = NULL;
	QfAudioThread *audio_thread = NULL;
};

