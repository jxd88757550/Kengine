#include <QApplication>
#include "mainwindow.h"
#include "File.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	MainWindow w;
	w.show();

	return app.exec();
	
	std::cout << "Test\n";
}