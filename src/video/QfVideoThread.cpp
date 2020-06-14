#include "QfVideoThread.h"
// #include "QfVideoThread.h"
// #include "QFDecode.h"
#include <iostream>
using namespace std;
//�򿪣����ܳɹ��������
bool QfVideoThread::open(AVCodecParameters *para, QFVideoCallback *call, int width, int height)
{
	if (!para)
		return false;
	clear();
	video_mux.lock();
	synpts = 0;
	//��ʼ����ʾ����
	this->call = call;
	if (call)
	{
		call->init(width, height);
	}
	video_mux.unlock();
// 	//�򿪽�����
// 	if (!decode) 
// 		decode = new QFDecode();
	int re = true;
	if (!decode->open(para))
	{
		cout << "audio QFDecode open failed!" << endl;
		re = false;
	}
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

// void QfVideoThread::push(AVPacket *pkt)
// {
// 	if (!pkt)return;
// 	//����
// 	while (!isExit)
// 	{
// 		video_mux.lock();
// 		if (packs.size() < maxList)
// 		{
// 			packs.push_back(pkt);
// 			video_mux.unlock();
// 			break;
// 		}
// 		video_mux.unlock();
// 		msleep(1);
// 	}
// }

void QfVideoThread::run()
{
	while (!is_exit)
	{
		video_mux.lock();
		if (synpts < decode->get_pts()) {
			video_mux.unlock();
			msleep(1);
			continue;
		}
// 		//û������
// 		if (packs.empty() || !decode)
// 		{
// 			video_mux.unlock();
// 			msleep(1);
// 			continue;
// 		}
		AVPacket *pkt = pop();
// 		AVPacket *pkt = packs.front();
// 		packs.pop_front();
		bool re = decode->send(pkt);
		if (!re)
		{
			video_mux.unlock();
			msleep(1);
			continue;
		}
		//һ��send ���recv
		while (!is_exit)
		{
			AVFrame * frame = decode->recv();
			if (!frame) break;
			//��ʾ��Ƶ
			if (call)
			{
				call->repaint(frame);
			}

		}
		video_mux.unlock();
	}
}

// QfVideoThread::QfVideoThread()
// {
// }


QfVideoThread::~QfVideoThread()
{
	//�ȴ��߳��˳�
// 	is_exit = true;
// 	wait();
}

long long QfVideoThread::get_synpts() const
{
	return synpts;
}

void QfVideoThread::set_synpts(const long long& pts)
{
	synpts = pts;
}
