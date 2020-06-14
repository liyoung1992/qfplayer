#include "QfVideoThread.h"
// #include "QfVideoThread.h"
// #include "QFDecode.h"
#include <iostream>
using namespace std;
//打开，不管成功与否都清理
bool QfVideoThread::open(AVCodecParameters *para, QFVideoCallback *call, int width, int height)
{
	if (!para)return false;
	mux.lock();

	//初始化显示窗口
	this->call = call;
	if (call)
	{
		call->init(width, height);
	}

	//打开解码器
	if (!decode) 
		decode = new QFDecode();
	int re = true;
	if (!decode->open(para))
	{
		cout << "audio QFDecode open failed!" << endl;
		re = false;
	}
	mux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void QfVideoThread::push(AVPacket *pkt)
{
	if (!pkt)return;
	//阻塞
	while (!isExit)
	{
		mux.lock();
		if (packs.size() < maxList)
		{
			packs.push_back(pkt);
			mux.unlock();
			break;
		}
		mux.unlock();
		msleep(1);
	}
}

void QfVideoThread::run()
{
	while (!isExit)
	{
		mux.lock();

		//没有数据
		if (packs.empty() || !decode)
		{
			mux.unlock();
			msleep(1);
			continue;
		}

		AVPacket *pkt = packs.front();
		packs.pop_front();
		bool re = decode->send(pkt);
		if (!re)
		{
			mux.unlock();
			msleep(1);
			continue;
		}
		//一次send 多次recv
		while (!isExit)
		{
			AVFrame * frame = decode->recv();
			if (!frame) break;
			//显示视频
			if (call)
			{
				call->repaint(frame);
			}

		}
		mux.unlock();
	}
}

// QfVideoThread::QfVideoThread()
// {
// }


QfVideoThread::~QfVideoThread()
{
	//等待线程退出
	isExit = true;
	wait();
}
