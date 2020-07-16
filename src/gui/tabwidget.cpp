#include "tabwidget.h"
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
