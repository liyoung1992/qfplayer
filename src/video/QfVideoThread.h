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
	//�򿪣����ܳɹ��������
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
	//������
	//int maxList = 100;
	//bool isExit = false;
	//long long pts = 0;
	//ͬ��ʱ�䣬�ⲿ������
	long long synpts = 0;
};
#endif // QFVIDEOTHREAD


