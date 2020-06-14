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

	//清理队列
	virtual void clear();

	//取出一帧数据，并出栈，如果没有返回NULL
	virtual AVPacket *pop();

protected:
	std::list <AVPacket *> packs;
	std::mutex mux;
	QFDecode *decode = 0;
	//最大队列
	int max_list = 100;
	bool is_exit = false;
};

