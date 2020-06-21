#include "qfplayer.h"
#include <QFileDialog>
#include "demux/QfDemuxThread.h"
#include <QMessageBox>
static QfDemuxThread dt;


qfplayer::qfplayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	dt.start();
	startTimer(40);
	//connect(ui.playPos,SIGNAL(pressed()));
	//ui.video->resize(this->size());
}

qfplayer::~qfplayer()
{
	dt.close();
}

void qfplayer::setPause(const bool& is_pause)
{
	if (is_pause) {
		ui.playBtn->setIcon(QIcon(":/qfplayer/resource/images/video_stop.svg"));
	}
	else {
		ui.playBtn->setIcon(QIcon(":/qfplayer/resource/images/video_play.svg"));
	}
}

void qfplayer::timerEvent(QTimerEvent *event)
{
	if (is_slider_press)
		return;
	long long total = dt.get_total_ms();
	if (total > 0)
	{
		double pos = (double)dt.get_pts() / (double)total;
		int v = ui.playPos->maximum() * pos;
		ui.playPos->setValue(v);
	}
}

void qfplayer::resizeEvent(QResizeEvent *event)
{
	ui.video->resize(this->size());
}

void qfplayer::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (isFullScreen()) {
		QRect rect = this->normalGeometry();
		this->showNormal();
		ui.video->resize(QSize(rect.width(),
			rect.height() - (ui.openFile->height() + ui.playPos->height())));
	}

	else
		this->showFullScreen();
}

void qfplayer::on_openFile_clicked()
{
	//选择文件
	QString name = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("选择视频文件"));
	if (name.isEmpty())
		return;
	this->setWindowTitle(name);
	if (!dt.open(name.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(0, "error", "open file failed!");
		return;
	}
	setPause(false);
}

void qfplayer::on_fullScreen_clicked()
{
	if (isFullScreen()) {
		QRect rect = this->normalGeometry();
		this->showNormal();
		ui.video->resize(QSize(rect.width(),
			rect.height()-(ui.openFile->height() + ui.playPos->height())));
	}
		
	else
		this->showFullScreen();
}

void qfplayer::on_playBtn_clicked()
{
	bool is_pause = !dt.get_pause();
	setPause(is_pause);
	dt.set_pause(is_pause);
}

void qfplayer::on_playPos_sliderPressed()
{
	is_slider_press = true;
// 	double pos = (double)e->pos().x() / (double)width();
// 	ui.playPos->setValue(pos * ui.playPos->maximum());
}

void qfplayer::on_playPos_sliderReleased()
{
	is_slider_press = false;
	double pos = 0.0;
	pos = (double)ui.playPos->value() / (double)ui.playPos->maximum();
	dt.seek(pos);

}
