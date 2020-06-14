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


//ȡ��һ֡���ݣ�����ջ�����û�з���NULL
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
	//����
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
	//�򿪽�����
	if (!decode)
		decode = new QFDecode();
}


QfDecodeThread::~QfDecodeThread()
{	//�ȴ��߳��˳�
	is_exit = true;
	wait();
}