#include "buttonswidget.h"

asclepios::gui::ButtonsWidget::ButtonsWidget(QWidget *parent)
	: QWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ButtonsWidget::initView()
{
	m_ui.setupUi(this);
}
