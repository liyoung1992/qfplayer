#ifndef QFVIDEOTHREAD
#define QFVIDEOTHREAD
#include <QThread>
#include "../utils/pch.h"
#include "../utils/function.h"
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

	virtual bool repaint_to_pts(AVPacket *pkt, long long seekpts);

	QfVideoThread() = default;
	virtual ~QfVideoThread();

	long long get_synpts() const;
	void  set_synpts(const long long& pts);

	void set_pause(const bool& pause);
	bool get_pause() const;
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
	bool is_pause = false;
};
#endif // QFVIDEOTHREAD


