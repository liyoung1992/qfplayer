#ifndef QFPLAYER_H 
#define  QFPLAYER_H


#include <QtWidgets/QWidget>
#include "ui_qfplayer.h"

class qfplayer : public QWidget
{
	Q_OBJECT

public:
	qfplayer(QWidget *parent = Q_NULLPTR);

public slots:
	void on_openFile_clicked();
private:
	Ui::qfplayerClass ui;
};
#endif // QFPLAYER_H