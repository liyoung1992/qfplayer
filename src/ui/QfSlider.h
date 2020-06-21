#pragma once
#include <QObject>
#include <QMouseEvent>
#include <QSlider>

class QfSlider : public QSlider
{
	Q_OBJECT

public:
	QfSlider(QWidget *parent);
	~QfSlider();
protected:
	void mousePressEvent(QMouseEvent *ev);
};
