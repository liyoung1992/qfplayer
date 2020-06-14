#pragma once
#include <qthread.h>
#include "../utils/pch.h"
#include "QfDecode.h"
class QfDecodeThread :
	public QThread
{
public:
	QfDecodeThread() ;
	virtual ~QfDecodeThread();
	virtual void push(AVPacket *pkt);

	//�������
	virtual void clear();

	//ȡ��һ֡���ݣ�����ջ�����û�з���NULL
	virtual AVPacket *pop();

protected:
	std::list <AVPacket *> packs;
	std::mutex mux;
	QFDecode *decode = 0;
	//������
	int max_list = 100;
	bool is_exit = false;
};

