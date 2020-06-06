#include "qfplayer.h"
#include <QtWidgets/QApplication>
#include <iostream>

#include "demux/QfDemux.h"
int main(int argc, char *argv[])
{
	///≤‚ ‘demux
	QFDemux demux;
	char* url = "D://test.mp4";
	std::cout << "demux.Open = " << demux.open(url);


	QApplication a(argc, argv);
	qfplayer w;
	w.show();
	return a.exec();
}
