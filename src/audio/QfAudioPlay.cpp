#include "QfAudioPlay.h"

#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <mutex>
class CQfAudioPlay :public QfAudioPlay
{
public:
	QAudioOutput *output = NULL;
	QIODevice *io = NULL;
	std::mutex mux;
	bool is_pause = false;
	virtual long long get_remainder_ms() {
		mux.lock();
		if (!output) {
			mux.unlock();
			return 0;
		}
		long long pts = 0;
		//未播放的字节数

		double size = output->bufferSize() -
			output->bytesFree();
		//一秒音频字节大小
		double secSize = sampleRate * (sampleSize / 8)*channels;
		if (secSize <= 0) {
			pts = 0;
		}
		else
		{
			pts = (size / secSize) * 1000;
		}
		mux.unlock();
		return pts;
	}
	virtual void close()
	{
		mux.lock();
		if (io)
		{
			io->close();
			io = NULL;
		}
		if (output)
		{
			output->stop();
			delete output;
			output = 0;
		}
		mux.unlock();
	}
	virtual bool open()
	{
		close();
		QAudioFormat fmt;
		fmt.setSampleRate(sampleRate);
		fmt.setSampleSize(sampleSize);
		fmt.setChannelCount(channels);
		fmt.setCodec("audio/pcm");
		fmt.setByteOrder(QAudioFormat::LittleEndian);
		fmt.setSampleType(QAudioFormat::UnSignedInt);
		mux.lock();
		output = new QAudioOutput(fmt);
		io = output->start(); //开始播放
		mux.unlock();
		if (io)
			return true;
		return false;
	}
	virtual bool write(const unsigned char *data, int datasize)
	{
		if (!data || datasize <= 0)return false;
		mux.lock();
		if (!output || !io)
		{
			mux.unlock();
			return false;
		}
		int size = io->write((char *)data, datasize);
		mux.unlock();
		if (datasize != size)
			return false;
		return true;
	}
	virtual void clear() {
		mux.lock();
		if (io)
		{
			io->reset();
		}
		mux.unlock();
	}

	virtual int getFree()
	{
		mux.lock();
		if (!output)
		{
			mux.unlock();
			return 0;
		}
		int free = output->bytesFree();
		mux.unlock();
		return free;
	}
	virtual void set_pause(const bool& pause) {
		is_pause = pause;
		mux.lock();
		if (!output)
		{
			mux.unlock();
			return;
		}
		if (pause)
		{
			output->suspend();
		}
		else
		{
			output->resume();
		}
		mux.unlock();
	}
};
QfAudioPlay *QfAudioPlay::instance()
{
	static CQfAudioPlay play;
	return &play;
}

QfAudioPlay::QfAudioPlay()
{
}


QfAudioPlay::~QfAudioPlay()
{
}
