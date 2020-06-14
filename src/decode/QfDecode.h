#ifndef QFDECODE_H
#define QFDECODE_H

#include "../utils/pch.h"
#include <mutex>
#include <iostream>
class QFDecode {
public:
	QFDecode();
	virtual ~QFDecode();

	//打开解码器,不管成功与否都释放para空间
	virtual bool open(AVCodecParameters *para);

	virtual void close();
	virtual void clear();


	//发送到解码线程，不管成功与否都释放pkt空间（对象和媒体内容）
	virtual bool send(AVPacket *pkt);
	//获取解码数据，一次send可能需要多次Recv，
	//获取缓冲中的数据Send NULL在Recv多次
   //每次复制一份，由调用者释放 av_frame_free
	virtual AVFrame* recv();

	long long get_pts() const;

private:
	bool m_pIsAudio;
	std::mutex m_pMutex;
	AVCodecContext* m_pCodec;
	long long pts = 0;
};

#endif //QFDECODE_H