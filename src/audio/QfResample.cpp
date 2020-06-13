#include "QfResample.h"

QfResample::QfResample()
	: m_pSwrContext(NULL)
	, m_pOutFormat(1)
{

}

QfResample::~QfResample()
{

}

bool QfResample::open(AVCodecParameters *para)
{
	if (!para)return false;
	m_pMutex.lock();
	//音频重采样 上下文初始化
	//if(!m_pSwrContext)
	//	m_pSwrContext = swr_alloc();

	//如果m_pSwrContext为NULL会分配空间
	m_pSwrContext = swr_alloc_set_opts(m_pSwrContext,
		av_get_default_channel_layout(2),	//输出格式
		(AVSampleFormat)m_pOutFormat,			//输出样本格式 1 AV_SAMPLE_FMT_S16
		para->sample_rate,					//输出采样率
		av_get_default_channel_layout(para->channels),//输入格式
		(AVSampleFormat)para->format,
		para->sample_rate,
		0, 0
	);
	avcodec_parameters_free(&para);
	int re = swr_init(m_pSwrContext);
	m_pMutex.unlock();
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		std::cout << "swr_init  failed! :" << buf << std::endl;
		return false;
	}
	//unsigned char *pcm = NULL;
	return true;
}

void QfResample::close()
{
	m_pMutex.lock();
	if (m_pSwrContext)
		swr_free(&m_pSwrContext);

	m_pMutex.unlock();
}

int QfResample::resample(AVFrame *indata, unsigned char *d)
{
	if (!indata) return 0;
	if (!d)
	{
		av_frame_free(&indata);
		return 0;
	}
	uint8_t *data[2] = { 0 };
	data[0] = d;
	int re = swr_convert(m_pSwrContext,
		data, indata->nb_samples,		//输出
		(const uint8_t**)indata->data, indata->nb_samples	//输入
	);
	if (re <= 0)return re;
	int outSize = re * indata->channels * 
		av_get_bytes_per_sample((AVSampleFormat)m_pOutFormat);
	return outSize;
}
