#ifndef QFDEMUX_H
#define QFDEMUX_H
#include "../utils/pch.h"
#include <mutex>
class QFDemux
{
public:
	QFDemux();
	virtual ~QFDemux();

	//��ý���ļ���������ý��
	virtual bool open(const char *url);

private:
	//����Ƶ��������ȡʱ����
	int m_pVideoStream;
	int m_pAduioStream;
	// ý����ʱ��
	int m_pTotalMs;
	std::mutex m_pMutex;
	AVFormatContext* m_pAvFromatContext;
};


#endif // QFDEMUX_H


