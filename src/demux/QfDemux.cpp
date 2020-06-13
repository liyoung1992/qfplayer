#include "QFDemux.h"
#include <iostream>
#include "../utils/function.h"
QFDemux::QFDemux()
	: m_pAvFromatContext(NULL)
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


	m_pWidth = as->codecpar->width;
	m_pHeight = as->codecpar->height;

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

	m_pSampleRate = as->codecpar->sample_rate;
	m_pChannels = as->codecpar->channels;

	//AVSampleFormat;
	std::cout << "channels = " << as->codecpar->channels << std::endl;
	//һ֡���ݣ��� ��ͨ�������� 
	std::cout << "frame_size = " << as->codecpar->frame_size << std::endl;
	//1024 * 2 * 2 = 4096  fps = sample_rate/frame_size

	m_pMutex.unlock();

	return true;
}

AVPacket* QFDemux::read()
{
	m_pMutex.lock();
	if (!m_pAvFromatContext) {
		m_pMutex.unlock();
		return false;
	}
	AVPacket *pkt = av_packet_alloc();
	//��ȡһ֡��������ռ�
	int re = av_read_frame(m_pAvFromatContext, pkt);
	if (re != 0)
	{
		m_pMutex.unlock();
		av_packet_free(&pkt);
		return 0;
	}
	//ptsת��Ϊ����
	pkt->pts = pkt->pts*(1000 *
		(r2d(m_pAvFromatContext->streams[pkt->stream_index]->time_base)));
	pkt->dts = pkt->dts*(1000 *
		(r2d(m_pAvFromatContext->streams[pkt->stream_index]->time_base)));
	m_pMutex.unlock();
	std::cout << pkt->pts << " " << std::flush;
	return pkt;
}

bool QFDemux::seek(double pos)
{
	m_pMutex.lock();
	if (!m_pAvFromatContext) {
		m_pMutex.unlock();
		return false;
	}
	//�����ȡ����
	avformat_flush(m_pAvFromatContext);
	long long seek_pos = 0;
	seek_pos = m_pAvFromatContext->streams[m_pVideoStream]->duration*pos;
	int result = av_seek_frame(m_pAvFromatContext,m_pVideoStream,seek_pos,
		AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
	m_pMutex.unlock();
	if (result < 0)
		return false;
	return true;
}

void QFDemux::clear()
{
	m_pMutex.lock();
	if (!m_pAvFromatContext)
	{
		m_pMutex.unlock();
		return;
	}
	//�����ȡ����
	avformat_flush(m_pAvFromatContext);
	m_pMutex.unlock();
}

void QFDemux::close()
{
	m_pMutex.lock();
	if (!m_pAvFromatContext)
	{
		m_pMutex.unlock();
		return;
	}
	avformat_close_input(&m_pAvFromatContext);
	//ý����ʱ�������룩
	m_pTotalMs = 0;
	m_pMutex.unlock();
}

AVCodecParameters * QFDemux::copyVideoPara()
{
	m_pMutex.lock();
	if (!m_pAvFromatContext)
	{
		m_pMutex.unlock();
		return NULL;
	}
	AVCodecParameters *pa = avcodec_parameters_alloc();
	avcodec_parameters_copy(pa, m_pAvFromatContext->streams[m_pVideoStream]->codecpar);
	m_pMutex.unlock();
	return pa;
}



AVCodecParameters * QFDemux::copyAudioPara()
{
	m_pMutex.lock();
	if (!m_pAvFromatContext)
	{
		m_pMutex.unlock();
		return NULL;
	}
	AVCodecParameters *pa = avcodec_parameters_alloc();
	avcodec_parameters_copy(pa, m_pAvFromatContext->streams[m_pAduioStream]->codecpar);
	m_pMutex.unlock();
	return pa;
}

bool QFDemux::isAudio(AVPacket* pkt)
{
	if (!pkt)
		return false;
	if (pkt->stream_index == m_pVideoStream)
		return false;
	return true;
}

int QFDemux::width() const
{
	return m_pWidth;
}

int QFDemux::height() const
{
	return m_pHeight;
}

int QFDemux::sample_rate() const
{
	return m_pSampleRate;
}

int QFDemux::channels() const
{
	return m_pChannels;
}
