#ifndef QFAUDIOTHREAD_H
#define QFAUDIOTHREAD_H
#include <QThread>
#include "../utils/pch.h"
#include "../decode/QfDecode.h"
#include "QfAudioPlay.h"
#include "QfResample.h"
class QfAudioThread : public QThread
{
public:
	QfAudioThread() = default;
	virtual ~QfAudioThread();

	virtual bool open(AVCodecParameters *para,
		int sampleRate, int channels);

	virtual void push(AVPacket* pkt);

	void run();

private:
	std::list<AVPacket*> packs;
	std::mutex mutex;
	QFDecode* decode = NULL;
	QfAudioPlay* audio_play = NULL;
	QfResample* resample = NULL;
	bool is_exit = false;
	int max_list = 100;
};

#endif // QFAUDIOTHREAD_H
