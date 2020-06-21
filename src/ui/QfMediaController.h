#pragma once

#include <QWidget>
#include "ui_QfMediaController.h"

class QfMediaController : public QWidget
{
	Q_OBJECT

public:
	QfMediaController(QWidget *parent = Q_NULLPTR);
	~QfMediaController();

private:
	Ui::QfMediaController ui;
};
