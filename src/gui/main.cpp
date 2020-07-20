//#include <vld.h>
#include <vtkAutoInit.h>
#include "gui.h"
#include "guiframe.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

int main(int argc, char* argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication application(argc, argv);
	application.setWindowIcon(QIcon(iconapp));
	asclepios::gui::GUIFrame guiFrame;
	asclepios::gui::GUI gui;
	guiFrame.setContent(&gui);
	guiFrame.showMaximized();
	return application.exec();
}
