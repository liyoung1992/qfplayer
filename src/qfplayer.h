#ifndef QFPLAYER_H 
#define  QFPLAYER_H


#include <QtWidgets/QWidget>
#include "ui_qfplayer.h"

class qfplayer : public QWidget
{
	Q_OBJECT

public:
	qfplayer(QWidget *parent = Q_NULLPTR);
	virtual ~qfplayer();
	void setPause(const bool& is_pause);
protected:
	void timerEvent(QTimerEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	
public slots:
	void on_openFile_clicked();
	void on_fullScreen_clicked();
	void on_playBtn_clicked();
	void on_playPos_sliderPressed();
	void on_playPos_sliderReleased();
private:
	Ui::qfplayerClass ui;
	bool is_slider_press = false;
};
#endif // QFPLAYER_H