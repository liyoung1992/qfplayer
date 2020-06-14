#include "QfDecode.h"

QFDecode::QFDecode()
	: m_pCodec(NULL)
	, m_pIsAudio(false)
{

}

QFDecode::~QFDecode()
{

}

bool QFDecode::open(AVCodecParameters *para)
{
	if (!para)
		return false;
	close();
	pts = 0;
	///解码器打开
	///找到解码器
	AVCodec *vcodec = avcodec_find_decoder(para->codec_id);
	if (!vcodec) {
		avcodec_parameters_free(&para);
		std::cout << "can't find the codec id " <<
			para->codec_id << std::endl;
		return false;
	}
	std::cout << "find the AVCodec " << para->codec_id << std::endl;
	m_pMutex.lock();
	m_pCodec = avcodec_alloc_context3(vcodec);
	///配置解码器上下文参数
	avcodec_parameters_to_context(m_pCodec, para);
	avcodec_parameters_free(&para);
	//八线程解码
	m_pCodec->thread_count = 8;

	///打开解码器上下文
	int re = avcodec_open2(m_pCodec, 0, 0);
	if (re != 0)
	{
		avcodec_free_context(&m_pCodec);
		m_pMutex.unlock();
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		std::cout << "avcodec_open2  failed! :" << buf << std::endl;
		return false;
	}
	m_pMutex.unlock();
	std::cout << " avcodec_open2 success!" << std::endl;
	return true;
}

void QFDecode::close()
{
	m_pMutex.lock();
	if (m_pCodec)
	{
		avcodec_close(m_pCodec);
		avcodec_free_context(&m_pCodec);
	}
	pts = 0;
	m_pMutex.unlock();
}

void QFDecode::clear()
{
	m_pMutex.lock();
	//清理解码缓冲
	if (m_pCodec)
		avcodec_flush_buffers(m_pCodec);
	m_pMutex.unlock();
}

bool QFDecode::send(AVPacket *pkt)
{
	if (!pkt || pkt->size <= 0 || !pkt->data)
		return false;
	m_pMutex.lock();
	if (!m_pCodec) {
		m_pMutex.unlock();
		return false;
	}
	int result = avcodec_send_packet(m_pCodec, pkt);
	m_pMutex.unlock();
	av_packet_free(&pkt);
	if (result != 0)
		return false;
	return true;
}

AVFrame* QFDecode::recv()
{
	m_pMutex.lock();
	if (!m_pCodec) {
		m_pMutex.unlock();
		return NULL;
	}
	AVFrame* frame = av_frame_alloc();
	int result = avcodec_receive_frame(m_pCodec,frame);
	m_pMutex.unlock();
	if (result != 0) {
		av_frame_free(&frame);
		return NULL;
	}
	pts = frame->pts;
	//std::cout << "[" << frame->linesize[0] << "] " << std::flush;
	return frame;
}

long long QFDecode::get_pts() const
{
	return pts;
}
