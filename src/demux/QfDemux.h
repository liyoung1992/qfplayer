#ifndef QFDEMUX_H
#define QFDEMUX_H
#include "../utils/pch.h"
#include <mutex>
//�⸴��
class QFDemux
{
public:
	QFDemux();
	virtual ~QFDemux();

	//��ý���ļ���������ý��
	virtual bool open(const char *url);

	//��ȡ��Ƶ֡
	//�ռ���Ҫ�������ͷ� ���ͷ�AVPacket����ռ䣬
	//�����ݿռ� av_packet_free
	virtual AVPacket*  read();


	//seek (0-1)
	virtual bool seek(double pos);

	//��ն�ȡ����
	virtual void clear();
	virtual void close();
	//��ȡ��Ƶ����  
	//���صĿռ���Ҫ����  avcodec_parameters_free
	AVCodecParameters *copyVideoPara();

	//��ȡ��Ƶ����  
	//���صĿռ���Ҫ���� avcodec_parameters_free
	AVCodecParameters *copyAudioPara();

	virtual bool isAudio(AVPacket* pkt);

	int width() const;
	int height() const;
	int sample_rate() const;
	int channels() const;
private:
	//����Ƶ��������ȡʱ����
	int m_pVideoStream;
	int m_pAduioStream;
	int m_pHeight;
	int m_pWidth;
	int m_pSampleRate = 0;
	int m_pChannels = 0;
	// ý����ʱ��
	int m_pTotalMs;
	std::mutex m_pMutex;
	AVFormatContext* m_pAvFromatContext;
};


#endif // QFDEMUX_H


