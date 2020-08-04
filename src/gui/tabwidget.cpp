#include "tabwidget.h"
#include <QFocusEvent>
#include <qstyle.h>
#include <QTabBar>
#include <QMimeData>
#include <QJsonDocument>
#include <study.h>
#include <patient.h>
#include "widget2d.h"

asclepios::gui::TabWidget::TabWidget(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	setStyleSheet(inActiveTabStyle);
	setAcceptDrops(true);
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
void asclepios::gui::TabWidget::resetWidget()
{
	auto* const widget = dynamic_cast<Widget2D*>(m_tabbedWidget);
	widget->getFuture().waitForFinished();
	delete m_tabbedWidget;
	m_tabbedWidget = nullptr;
	while (m_ui.tab->count())
	{
		delete m_ui.tab->widget(0);
	}
	createWidget2D();
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
	if (m_isActive)
	{
		tab->setStyleSheet(activeTabStyle);
		setStyleSheet(activeTabStyle);
		emit focused(this);
	}
	else
	{
		tab->setStyleSheet(inActiveTabStyle);
		setStyleSheet(inActiveTabStyle);
	}
	style()->unpolish(tab);
	style()->polish(tab);
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

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept();
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::dropEvent(QDropEvent* event)
{
	const QString data = event->mimeData()->text();
	if (data.isEmpty())
	{
		return;
	}
	auto const [series, image] = getDropData(data);
	if(!series || ! image)
	{
		return;
	}
	resetWidget();
	populateWidget(series, image);
}

//-----------------------------------------------------------------------------
void asclepios::gui::TabWidget::populateWidget(core::Series* t_series, core::Image* t_image)
{
	auto* const widget = dynamic_cast<Widget2D*>(m_tabbedWidget);
	if (!widget)
	{
		return;
	}
	widget->setSeries(t_series);
	widget->setImage(t_image);
	auto* const study = t_series->getParentObject();
	widget->setIndexes(study->getParentObject()->getIndex(),
		study->getIndex(), t_series->getIndex(),
		t_image->getIndex());
	widget->setIsImageLoaded(true);
	widget->render();
	//todo connect refresh slider
	if(!m_isActive)
	{
		auto* ev = new QFocusEvent(QEvent::FocusIn,
			Qt::FocusReason::MouseFocusReason);
		focusInEvent(ev);
		delete ev;
	}
}

//-----------------------------------------------------------------------------
std::tuple<asclepios::core::Series*, asclepios::core::Image*> asclepios::gui::TabWidget::getDropData(
	const QString& t_data)
{
	const auto jsonDoc = QJsonDocument::fromJson(t_data.toUtf8());
	const auto obj = jsonDoc.object();
	return std::make_tuple(
		reinterpret_cast<core::Series*>(obj["Series"].toString().toULongLong(nullptr, 16)),
		reinterpret_cast<core::Image*>(obj["Image"].toString().toULongLong(nullptr, 16)));
}
