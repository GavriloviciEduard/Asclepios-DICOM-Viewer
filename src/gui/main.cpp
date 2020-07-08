#include "gui.h"
#include <QtWidgets/QApplication>
#include "guiframe.h"

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);
	application.setWindowIcon(QIcon(iconapp));
	asclepios::gui::GUIFrame guiFrame;
	asclepios::gui::GUI gui;
	guiFrame.setContent(&gui);
	guiFrame.show();
	return application.exec();
}
