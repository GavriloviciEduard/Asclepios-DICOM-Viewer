#include "LayoutMenu.h"

asclepios::gui::LayoutMenu::LayoutMenu(QWidget* parent)
	: QMenu(parent)
{
	initView();
}

void asclepios::gui::LayoutMenu::initView()
{
	m_ui.setupUi(this);
}
