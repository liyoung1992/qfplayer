#ifndef QFVIDEOTHREAD
#define QFVIDEOTHREAD
#include <QThread>
#include "../utils/pch.h"
#include "QfVideoCallback.h"
#include "../decode/QfDecode.h"
#include "../decode/QfDecodeThread.h"
class QfVideoThread :
	public QfDecodeThread
{
public:
	//打开，不管成功与否都清理
	virtual bool open(AVCodecParameters *para,
		QFVideoCallback *call, int width, int height);
//	virtual void push(AVPacket *pkt);
	void run();

	QfVideoThread() = default;
	virtual ~QfVideoThread();

	long long get_synpts() const;
	void  set_synpts(const long long& pts);
protected:
	//std::list <AVPacket *> packs;
	std::mutex video_mux;
	//QFDecode *decode = NULL;
	QFVideoCallback *call = NULL;
	//最大队列
	//int maxList = 100;
	//bool isExit = false;
	//long long pts = 0;
	//同步时间，外部传进来
	long long synpts = 0;
};
#endif // QFVIDEOTHREAD


