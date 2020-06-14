#ifndef QFVIDEOCALLBACK_H
#define QFVIDEOCALLBACK_H
#include "../utils/pch.h"
class QFVideoCallback {
public:
	virtual void init(const int& width,
		const int& height) = 0;
	virtual void repaint(AVFrame* frame) = 0;
};

#endif // QFVIDEOCALLBACK_H
