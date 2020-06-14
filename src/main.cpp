#include "qfplayer.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <QThread>
#include "demux/QfDemux.h"
#include "decode/QfDecode.h"
#include <QLabel>
#include "audio/QfResample.h"
#include "audio/QfAudioPlay.h"
#include "audio/QfAudioThread.h"
#include "video/QfVideoThread.h"
#include "demux/QfDemuxThread.h"

using namespace std;
class TestThread :public QThread
{
public:
	QfAudioThread at;
	QfVideoThread vt;
	void Init()
	{
		//œ„∏€Œ¿ ”
		char *url = "D:/test.mp4";
// 		std::cout << "demux.Open = " << demux.open(url);
// 		demux.read();
// 		demux.clear();
// 		demux.close();
		url = "D:/test.mp4";
		cout << "demux.Open = " << demux.open(url);
// 		cout << "CopyVPara = " << demux.copyVideoPara() << endl;
// 		cout << "CopyAPara = " << demux.copyAudioPara() << endl;
		//cout << "seek=" << demux.Seek(0.95) << endl;

		/////////////////////////////
// 		img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
// // 			pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
// 		cout << "vdecode.Open() = " << vdecode.open(demux.copyVideoPara()) << endl;
// 		//vdecode.Clear();
// 		//vdecode.Close();
// 		cout << "adecode.Open() = " << adecode.open(demux.copyAudioPara()) << endl;
// 		cout << "resample.Open = " << resample.open(demux.copyAudioPara()) << endl;
// 		QfAudioPlay::instance()->channels = demux.channels();
// 		QfAudioPlay::instance()->sampleRate = demux.sample_rate();
// 
// 		cout << "XAudioPlay::Get()->Open() = " << QfAudioPlay::instance()->open() << endl;
		cout << "at.Open = " << at.open(demux.copyAudioPara(), demux.sample_rate(), demux.channels());
		vt.open(demux.copyVideoPara(), video, demux.width(), demux.height());
		at.start();
		vt.start();
	}
	unsigned char *pcm = new unsigned char[1024 * 1024];
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.read();
			if (demux.isAudio(pkt))
			{
				at.push(pkt);
// 				adecode.send(pkt);
// 				AVFrame *frame = adecode.recv();
// // 				adecode.Send(pkt);
// // 				AVFrame *frame = adecode.Recv();
// 				int len = resample.resample(frame, pcm);
// 				//cout << "Resample:" << resample.resample(frame, pcm) << " ";
// 				cout << "Resample:" << len << " ";
// 				while (len > 0)
// 				{
// 					if (QfAudioPlay::instance()->getFree() >= len)
// 					{
// 						QfAudioPlay::instance()->write(pcm, len);
// 						break;
// 					}
// 					msleep(1);
// 				}
// 				//cout << "Audio:" << frame << endl;
			}
			else
			{
				vt.push(pkt);
// 				vdecode.send(pkt);
// 				AVFrame *frame = vdecode.recv();
// 				video->repaint(frame);
// 				msleep(40);
				//cout << "Video:" << frame << endl;
			}
			if (!pkt)
			{
				demux.seek(0);
				//break;
			}
		}
	}
	///≤‚ ‘XDemux
	QFDemux demux;
	///Ω‚¬Î≤‚ ‘
// 	QFDecode vdecode;
// 	QFDecode adecode;
	QFVideoWidget *video = NULL;
	//QfResample resample;
	//QLabel* label;
};

int main(int argc, char *argv[])
{
	///≤‚ ‘demux
// 	QFDemux demux;
// 	char* url = "D://cut.mp4";
// 	std::cout << "demux.Open = " << demux.open(url) << std::endl;
// 	std::cout << "CopyVPara = " << demux.copyVideoPara() << std::endl;
// 	std::cout << "CopyAPara = " << demux.copyAudioPara() << std::endl;
// 	std::cout << "seek=" << demux.seek(0.1) << std::endl;
// 
// 	QFDecode vdecode;
// 	std::cout << "vdecode.Open() = " << vdecode.open(demux.copyVideoPara()) << std::endl;
// // 	vdecode.clear();
// // 	vdecode.close();
// 	QFDecode adecode;
// 	std::cout << "adecode.Open() = " << adecode.open(demux.copyAudioPara()) << std::endl;
// 	for (;;)
// 	{
// 		AVPacket *pkt = demux.read();
// 		if (!pkt)break;
// 	}
// 	for (;;)
// 	{
// 		AVPacket *pkt = demux.read();
// 		if (demux.isAudio(pkt))
// 		{
// 			adecode.send(pkt);
// 			AVFrame *frame = adecode.recv();
// 			//cout << "Audio:" << frame << endl;
// 		}
// 		else
// 		{
// 			vdecode.send(pkt);
// 			AVFrame *frame = vdecode.recv();
// 			//cout << "Video:" << frame << endl;
// 		}
// 		if (!pkt)break;
// 	}

	//TestThread tt;



	QApplication a(argc, argv);
	qfplayer w;
	w.show();


	//≥ı ºªØgl¥∞ø⁄
// 	QfDemuxThread dt;
// 	dt.open("D:/test.mp4", w.ui.video);
// 	//dt.open("rtsp://192.168.1.110:8554/", w.ui.video);
// 	dt.start();
	return a.exec();
}
