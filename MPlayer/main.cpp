#include "mplayer.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MPlayer w;
	w.show();
	return a.exec();
}

