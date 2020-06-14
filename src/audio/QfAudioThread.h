#ifndef QFAUDIOTHREAD_H
#define QFAUDIOTHREAD_H
#include <QThread>
#include "../utils/pch.h"
#include "../decode/QfDecode.h"
#include "QfAudioPlay.h"
#include "QfResample.h"
#include "../decode/QfDecodeThread.h"
class QfAudioThread : public QfDecodeThread
{
public:
	QfAudioThread();
	virtual ~QfAudioThread();

	virtual bool open(AVCodecParameters *para,
		int sampleRate, int channels);

	//virtual void push(AVPacket* pkt);

	void run();

	long long get_pts() const;
private:
	//std::list<AVPacket*> packs;
	std::mutex audio_mutex;
	//QFDecode* decode = NULL;
	QfAudioPlay* audio_play = NULL;
	QfResample* resample = NULL;
	bool is_exit = false;
	//int max_list = 100;
	long long pts;
};

#endif // QFAUDIOTHREAD_H
