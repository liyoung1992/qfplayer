#include "QFDemux.h"
#include <iostream>
#include "../utils/function.h"
QFDemux::QFDemux()
{
	//��ʼ����װ��
	av_register_all();

	//��ʼ������� �����Դ�rtsp rtmp http Э�����ý����Ƶ��
	avformat_network_init();
}

QFDemux::~QFDemux()
{

}

bool QFDemux::open(const char *url)
{
	//��������
	AVDictionary *opt = NULL;
	av_dict_set(&opt, "rtsp_transport", "tcp", 0);
	//��ʱ����
	av_dict_set(&opt, "max_delay", "500", 0);

	m_pMutex.lock();

	int result = avformat_open_input(&m_pAvFromatContext,url,0,&opt);
	if (result != 0) {
		m_pMutex.unlock();
		char buf[1024] = { 0 };
		av_strerror(result, buf, sizeof(buf) - 1);
		std::cout << "open " << url << " failed! :" << buf << std::endl;
		return false;
	}
	std::cout << "open " << url << " success! " << std::endl;

	//��ȡ����Ϣ 
	result = avformat_find_stream_info(m_pAvFromatContext, 0);

	//��ʱ�� ����
	int totalMs = m_pAvFromatContext->duration / (AV_TIME_BASE / 1000);
	std::cout << "totalMs = " << totalMs << std::endl;

	//��ӡ��Ƶ����ϸ��Ϣ
	av_dump_format(m_pAvFromatContext, 0, url, 0);


	//��ȡ��Ƶ��
	m_pVideoStream = av_find_best_stream(m_pAvFromatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	AVStream *as = m_pAvFromatContext->streams[m_pVideoStream];

	std::cout << "=======================================================" << std::endl;
	std::cout << m_pVideoStream << "��Ƶ��Ϣ" << std::endl;
	std::cout << "codec_id = " << as->codecpar->codec_id << std::endl;
	std::cout << "format = " << as->codecpar->format << std::endl;
	std::cout << "width=" << as->codecpar->width << std::endl;
	std::cout << "height=" << as->codecpar->height << std::endl;
	//֡�� fps ����ת��
	std::cout << "video fps = " << r2d(as->avg_frame_rate) << std::endl;

	std::cout << "=======================================================" << std::endl;
	std::cout << m_pAduioStream << "��Ƶ��Ϣ" << std::endl;
	//��ȡ��Ƶ��
	m_pAduioStream = av_find_best_stream(m_pAvFromatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	as = m_pAvFromatContext->streams[m_pAduioStream];
	std::cout << "codec_id = " << as->codecpar->codec_id << std::endl;
	std::cout << "format = " << as->codecpar->format << std::endl;
	std::cout << "sample_rate = " << as->codecpar->sample_rate << std::endl;
	//AVSampleFormat;
	std::cout << "channels = " << as->codecpar->channels << std::endl;
	//һ֡���ݣ��� ��ͨ�������� 
	std::cout << "frame_size = " << as->codecpar->frame_size << std::endl;
	//1024 * 2 * 2 = 4096  fps = sample_rate/frame_size

	m_pMutex.unlock();

	return true;
}
