#include "QfVideoThread.h"
// #include "QfVideoThread.h"
// #include "QFDecode.h"
#include <iostream>
using namespace std;
//�򿪣����ܳɹ��������
bool QfVideoThread::open(AVCodecParameters *para, QFVideoCallback *call, int width, int height)
{
	if (!para)return false;
	mux.lock();

	//��ʼ����ʾ����
	this->call = call;
	if (call)
	{
		call->init(width, height);
	}

	//�򿪽�����
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
	//����
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

		//û������
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
		//һ��send ���recv
		while (!isExit)
		{
			AVFrame * frame = decode->recv();
			if (!frame) break;
			//��ʾ��Ƶ
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
	//�ȴ��߳��˳�
	isExit = true;
	wait();
}
