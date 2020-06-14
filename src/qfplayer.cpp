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
}

void qfplayer::on_openFile_clicked()
{
	//ѡ���ļ�
	QString name = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("ѡ����Ƶ�ļ�"));
	if (name.isEmpty())
		return;
	this->setWindowTitle(name);
	if (!dt.open(name.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(0, "error", "open file failed!");
		return;
	}
}
