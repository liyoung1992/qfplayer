#ifndef FUNCTION_H
#define  FUNCTION_H
#include "pch.h"

//֡�� fps ����ת��
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}
static void freePacket(AVPacket **pkt)
{
	if (pkt || (*pkt))
		return;
	av_packet_free(pkt);
}
#endif //FUNCTION_H
