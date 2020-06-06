#ifndef QFDEMUX_H
#define QFDEMUX_H
#include "../utils/pch.h"
#include <mutex>
class QFDemux
{
public:
	QFDemux();
	virtual ~QFDemux();

	//打开媒体文件、或者流媒体
	virtual bool open(const char *url);

private:
	//音视频索引，读取时区分
	int m_pVideoStream;
	int m_pAduioStream;
	// 媒体总时长
	int m_pTotalMs;
	std::mutex m_pMutex;
	AVFormatContext* m_pAvFromatContext;
};


#endif // QFDEMUX_H


