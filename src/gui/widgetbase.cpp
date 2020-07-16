#include "widgetbase.h"
#include <QFocusEvent>

asclepios::gui::WidgetBase::WidgetBase(QWidget* t_parent)
	: QWidget(t_parent), m_tabWidget(t_parent)
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetBase::focusInEvent(QFocusEvent* event)
{
	m_tabWidget->setFocus(event->reason());
	QWidget::focusInEvent(event);
}
