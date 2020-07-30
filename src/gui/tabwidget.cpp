#include "tabwidget.h"
#include <QFocusEvent>
#include <qstyle.h>
#include <QTabBar>
#include "widget2d.h"

asclepios::gui::TabWidget::TabWidget(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::createWidget2D()
{
	delete m_tabbedWidget;
	m_tabbedWidget = new Widget2D(this);
	m_tabbedWidget->setWidgetType(WidgetBase::WidgetType::widget2d);
	m_ui.tab->addTab(m_tabbedWidget, {}, "2D");
	m_ui.tab->setTabsClosable(true);
	m_ui.tab->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);
	m_ui.tab->tabBar()->setTabButton(0, QTabBar::LeftSide, nullptr);
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::resetWidget() const
{
	delete m_tabbedWidget;
	while (m_ui.tab->count())
	{
		delete m_ui.tab->widget(0);
	}
	//todo set title
}

//-----------------------------------------------------------------------------
asclepios::gui::WidgetBase::WidgetType asclepios::gui::TabWidget::getWidgetType() const
{
	return m_tabbedWidget
		       ? m_tabbedWidget->getWidgetType()
		       : WidgetBase::WidgetType::none;
}

//-----------------------------------------------------------------------------
asclepios::gui::WidgetBase* asclepios::gui::TabWidget::getActiveTabbedWidget() const
{
	return dynamic_cast<WidgetBase*>(m_ui.tab->widget(m_ui.tab->currentIndex()));
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::setTabTitle(const int& t_index, const QString& t_name) const
{
	m_ui.tab->setTabText(t_index, t_name);
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::onFocus(const bool& t_flag)
{
	m_isActive = t_flag;
	auto* const tab = dynamic_cast<QTabWidget*>(this->findChild<QTabWidget*>("tab"));
	tab->setProperty("active", t_flag);
	style()->unpolish(tab);
	style()->polish(tab);
	if (m_isActive)
	{
		emit focused(this);
	}
	update();
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::onMaximize()
{
	emit setMaximized(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::focusInEvent(QFocusEvent* event)
{
	if (event->reason() == Qt::FocusReason::MouseFocusReason
		&& !m_isActive)
	{
		onFocus(true);
	}
	QWidget::focusInEvent(event);
}
