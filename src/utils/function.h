#ifndef FUNCTION_H
#define  FUNCTION_H
#include "pch.h"

//帧率 fps 分数转换
static double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}
#endif //FUNCTION_H
