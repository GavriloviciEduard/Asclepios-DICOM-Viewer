#include "gui.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);
	asclepios::gui::GUI gui;
	gui.show();
	return application.exec();
}