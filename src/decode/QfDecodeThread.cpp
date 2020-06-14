#include "QfDecodeThread.h"
#include "../utils/function.h"

void QfDecodeThread::clear()
{
	mux.lock();
	decode->clear();
	while (!packs.empty())
	{
		AVPacket *pkt = packs.front();
		freePacket(&pkt);
		packs.pop_front();
	}
	mux.unlock();
}


//取出一帧数据，并出栈，如果没有返回NULL
AVPacket *QfDecodeThread::pop()
{
	mux.lock();
	if (packs.empty())
	{
		mux.unlock();
		return NULL;
	}
	AVPacket *pkt = packs.front();
	packs.pop_front();
	mux.unlock();
	return pkt;
}
void QfDecodeThread::push(AVPacket *pkt)
{
	if (!pkt)return;
	//阻塞
	while (!is_exit)
	{
		mux.lock();
		if (packs.size() < max_list)
		{
			packs.push_back(pkt);
			mux.unlock();
			break;
		}
		mux.unlock();
		msleep(1);
	}
}


QfDecodeThread::QfDecodeThread()
{
	//打开解码器
	if (!decode)
		decode = new QFDecode();
}


QfDecodeThread::~QfDecodeThread()
{	//等待线程退出
	is_exit = true;
	wait();
}