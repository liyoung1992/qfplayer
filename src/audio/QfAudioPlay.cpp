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
		io = output->start(); //¿ªÊ¼²¥·Å
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
