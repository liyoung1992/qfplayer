#ifndef QFDEMUX_H
#define QFDEMUX_H
#include "../utils/pch.h"
#include <mutex>
//解复用
class QFDemux
{
public:
	QFDemux();
	virtual ~QFDemux();

	//打开媒体文件、或者流媒体
	virtual bool open(const char *url);

	//读取视频帧
	//空间需要调用者释放 ，释放AVPacket对象空间，
	//和数据空间 av_packet_free
	virtual AVPacket*  read();


	//seek (0-1)
	virtual bool seek(double pos);

	//清空读取缓存
	virtual void clear();
	virtual void close();
	//获取视频参数  
	//返回的空间需要清理  avcodec_parameters_free
	AVCodecParameters *copyVideoPara();

	//获取音频参数  
	//返回的空间需要清理 avcodec_parameters_free
	AVCodecParameters *copyAudioPara();

	virtual bool isAudio(AVPacket* pkt);

	int width() const;
	int height() const;
	int sample_rate() const;
	int channels() const;
private:
	//音视频索引，读取时区分
	int m_pVideoStream;
	int m_pAduioStream;
	int m_pHeight;
	int m_pWidth;
	int m_pSampleRate = 0;
	int m_pChannels = 0;
	// 媒体总时长
	int m_pTotalMs;
	std::mutex m_pMutex;
	AVFormatContext* m_pAvFromatContext;
};


#endif // QFDEMUX_H


