#include "QfSlider.h"

QfSlider::QfSlider(QWidget *parent)
	: QSlider(parent)
{
}

QfSlider::~QfSlider()
{
}

void QfSlider::mousePressEvent(QMouseEvent *ev)
{
	double pos = (double)ev->pos().x() / (double)width();
	setValue(pos * this->maximum());

	QSlider::sliderReleased();
}
