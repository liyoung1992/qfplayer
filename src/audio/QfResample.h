#ifndef QFRESAMPLE_H
#define QFRESAMPLE_H
#include "../utils/pch.h"
#include <mutex>
#include <iostream>
class QfResample {
public:

	QfResample();
	~QfResample();

	//输出参数和输入参数一致除了采样格式，
	//输出为S16 ,会释放para
	virtual bool open(AVCodecParameters *para);
	virtual void close();

	//返回重采样后大小,不管成功与否都释放indata空间
	virtual int resample(AVFrame *indata, unsigned char *data);


protected:
	std::mutex m_pMutex;
	SwrContext *m_pSwrContext = 0;
	//AV_SAMPLE_FMT_S16
	int m_pOutFormat = 1;
};
#endif //QFRESAMPLE_H
