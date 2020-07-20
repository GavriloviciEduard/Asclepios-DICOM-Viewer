#include "imagefunctionstoolbar.h"

asclepios::gui::ImageFunctionsToolbar::ImageFunctionsToolbar(QWidget* parent)
	: QWidget(parent)
{
	initView();
}

void asclepios::gui::ImageFunctionsToolbar::initView()
{
	m_ui.setupUi(this);
}
