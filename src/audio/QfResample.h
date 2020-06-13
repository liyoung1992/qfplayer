#ifndef QFRESAMPLE_H
#define QFRESAMPLE_H
#include "../utils/pch.h"
#include <mutex>
#include <iostream>
class QfResample {
public:

	QfResample();
	~QfResample();

	//����������������һ�³��˲�����ʽ��
	//���ΪS16 ,���ͷ�para
	virtual bool open(AVCodecParameters *para);
	virtual void close();

	//�����ز������С,���ܳɹ�����ͷ�indata�ռ�
	virtual int resample(AVFrame *indata, unsigned char *data);


protected:
	std::mutex m_pMutex;
	SwrContext *m_pSwrContext = 0;
	//AV_SAMPLE_FMT_S16
	int m_pOutFormat = 1;
};
#endif //QFRESAMPLE_H
