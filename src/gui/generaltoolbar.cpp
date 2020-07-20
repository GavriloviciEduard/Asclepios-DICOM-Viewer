#include "GeneralToolbar.h"

asclepios::gui::GeneralToolbar::GeneralToolbar(QWidget *parent)
	: QWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GeneralToolbar::initView()
{
	m_ui.setupUi(this);
}
