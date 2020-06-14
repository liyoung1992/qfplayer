#ifndef QFDECODE_H
#define QFDECODE_H

#include "../utils/pch.h"
#include <mutex>
#include <iostream>
class QFDecode {
public:
	QFDecode();
	virtual ~QFDecode();

	//�򿪽�����,���ܳɹ�����ͷ�para�ռ�
	virtual bool open(AVCodecParameters *para);

	virtual void close();
	virtual void clear();


	//���͵������̣߳����ܳɹ�����ͷ�pkt�ռ䣨�����ý�����ݣ�
	virtual bool send(AVPacket *pkt);
	//��ȡ�������ݣ�һ��send������Ҫ���Recv��
	//��ȡ�����е�����Send NULL��Recv���
   //ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
	virtual AVFrame* recv();

	long long get_pts() const;

private:
	bool m_pIsAudio;
	std::mutex m_pMutex;
	AVCodecContext* m_pCodec;
	long long pts = 0;
};

#endif //QFDECODE_H