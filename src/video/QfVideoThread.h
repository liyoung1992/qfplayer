#ifndef QFVIDEOTHREAD
#define QFVIDEOTHREAD
#include <QThread>
#include "../utils/pch.h"
#include "QfVideoCallback.h"
#include "../decode/QfDecode.h"
class QfVideoThread :
	public QThread
{
public:
	//打开，不管成功与否都清理
	virtual bool open(AVCodecParameters *para,
		QFVideoCallback *call, int width, int height);
	virtual void push(AVPacket *pkt);
	void run();

	QfVideoThread() = default;
	virtual ~QfVideoThread();

protected:
	std::list <AVPacket *> packs;
	std::mutex mux;
	QFDecode *decode = NULL;
	QFVideoCallback *call = NULL;
	//最大队列
	int maxList = 100;
	bool isExit = false;
};
#endif // QFVIDEOTHREAD


