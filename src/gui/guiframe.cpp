#include "guiframe.h"
#include "utils.h"
#include <QPushButton>
#include <QDesktopWidget>

asclepios::gui::GUIFrame::GUIFrame(QWidget* parent) : QFrame(
	parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Dialog),
	m_currentScreen(QApplication::desktop()->screenNumber(this))
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::setContent(QWidget* t_widget)
{
	m_childWidget = t_widget;
	m_ui.widgetContent->layout()->addWidget(t_widget);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::updateMaximizeButton(const bool& maximized) const
{
	if (maximized)
	{
		m_ui.maximizeButton->setIcon(QIcon(buttonMaximizeOn));
		m_ui.maximizeButton->setToolTip(tr("Restore"));
	}
	else
	{
		m_ui.maximizeButton->setIcon(QIcon(buttonMaximizeOff));
		m_ui.maximizeButton->setToolTip(tr("Maximize"));
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::moveEvent(QMoveEvent* event)
{
	const auto screen =
		QApplication::desktop()->screenNumber(this);
	if (screen != m_currentScreen)
	{
		m_currentScreen = screen;
		resize(width() + 1, height());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::initView()
{
	m_ui.setupUi(this);
	setAutoFillBackground(false);
	setUpFramelessHelper();
	createConnections();
	m_ui.maximizeButton->setIcon(QIcon(buttonMaximizeOn));
	m_ui.icon->setPixmap(QPixmap::fromImage(QImage(iconTitleBar)));
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::createConnections() const
{
	Q_UNUSED(connect(m_ui.minimizeButton, &QPushButton::clicked,
		m_helper.get(), &FramelessHelper::triggerMinimizeButtonAction));
	Q_UNUSED(connect(m_ui.maximizeButton, &QPushButton::clicked,
		m_helper.get(), &FramelessHelper::triggerMaximizeButtonAction));
	Q_UNUSED(connect(m_ui.closeButton, &QPushButton::clicked,
		m_helper.get(), &FramelessHelper::triggerCloseButtonAction));
	Q_UNUSED(connect(m_helper.get(), &FramelessHelper::maximizedChanged,
		this, &GUIFrame::updateMaximizeButton));
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::setUpFramelessHelper()
{
	m_helper = std::make_unique<FramelessHelper>(this);
	m_helper->setDraggableMargins(3, 3, 3, 3);
	m_helper->setMaximizedMargins(3, 3, 3, 3);
	m_helper->setTitleBarHeight(32);
	m_helper->addExcludeItem(m_ui.minimizeButton);
	m_helper->addExcludeItem(m_ui.maximizeButton);
	m_helper->addExcludeItem(m_ui.closeButton);
}
