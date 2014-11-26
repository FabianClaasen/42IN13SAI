#include <QApplication>

#include "MainController.h"
#include "MainWindow.h"

int main(int argc, const char * argv[])
{
	QApplication app(argc, 0);
	std::unique_ptr<MainController> mainController(new MainController());
	app.exec();
}