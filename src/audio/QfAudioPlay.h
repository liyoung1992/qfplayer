#ifndef  QFAUDIOPLAY_H
#define QFAUDIOPLAY_H

class QfAudioPlay
{
public:


	static QfAudioPlay *instance();
	QfAudioPlay();
	virtual ~QfAudioPlay();

	//����Ƶ����
	virtual bool open() = 0;
	virtual void close() = 0;

	//������Ƶ
	virtual bool write(const unsigned char *data, int datasize) = 0;
	virtual int getFree() = 0;

	//���ػ����л�û�в��ŵ�ʱ��(����)
	virtual long long get_remainder_ms() = 0;
	int sampleRate = 44100;
	int sampleSize = 16;
	int channels = 2;
};
#endif // QFAUDIOPLAY_H


