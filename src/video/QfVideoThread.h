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
	//�򿪣����ܳɹ��������
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
	//������
	int maxList = 100;
	bool isExit = false;
};
#endif // QFVIDEOTHREAD


