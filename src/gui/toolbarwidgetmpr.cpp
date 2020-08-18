#include "toolbarwidgetmpr.h"
#include <QToolButton>

asclepios::gui::ToolbarWidgetMPR::ToolbarWidgetMPR(QWidget* parent)
	: QWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidgetMPR::onButtonPressed()
{
	auto* const button =
		dynamic_cast<QToolButton*>(sender());
	if (button->objectName() == "toolButtonReslicer")
	{
		emit activateResliceWidget(m_ui.toolButtonReslicer->isChecked());
	}
	else if (button->objectName() == "toolButtonReset")
	{
		emit resetResliceWidget();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidgetMPR::initView()
{
	m_ui.setupUi(this);
	createConnections();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidgetMPR::createConnections() const
{
	Q_UNUSED(connect(m_ui.toolButtonReset,
		&QToolButton::clicked,
		this, &ToolbarWidgetMPR::onButtonPressed));
	Q_UNUSED(connect(m_ui.toolButtonReslicer,
		&QToolButton::clicked,
		this, &ToolbarWidgetMPR::onButtonPressed));
}
