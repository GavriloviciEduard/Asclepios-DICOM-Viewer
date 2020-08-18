#include "guiframe.h"
#include <QDesktopWidget>
#include <QPushButton>
#include <QMenuBar>

#include "gui.h"
#include "utils.h"

asclepios::gui::GUIFrame::GUIFrame(QWidget* parent) :
	Frameless(parent)
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
void asclepios::gui::GUIFrame::changeEvent(QEvent* t_event)
{
	if (t_event->type() == QEvent::WindowStateChange)
	{
		auto* event = dynamic_cast<QWindowStateChangeEvent*>(t_event);
		if (event->oldState() & Qt::WindowMaximized && !isMaximized())
		{
			updateMaximizeButton(false);
		}
		else if (isMaximized())
		{
			updateMaximizeButton(true);
		}
	}
	t_event->accept();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onChangeLayout(const WidgetsContainer::layouts& t_layout) const
{
	dynamic_cast<GUI*>(m_childWidget)->onChangeLayout(t_layout);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onOpenFile() const
{
	dynamic_cast<GUI*>(m_childWidget)->onOpenFile();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onOpenFolder() const
{
	dynamic_cast<GUI*>(m_childWidget)->onOpenFolder();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onCloseAllPatients() const
{
	dynamic_cast<GUI*>(m_childWidget)->onCloseAllPatients();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onClose()
{
	close();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onMaximize()
{
	isMaximized() ? showNormal() : showMaximized();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::onMinimize()
{
	showMinimized();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::initView()
{
	m_ui.setupUi(this);
	setAutoFillBackground(false);
	m_ui.maximizeButton->setIcon(QIcon(buttonMaximizeOn));
	m_ui.icon->setPixmap(QPixmap::fromImage(QImage(iconTitleBar)));
	updateMaximizeButton(true);
	setResizeableAreaWidth(8);
	setTitleBar(m_ui.widgetTopBar);
	ignoreWidget(m_ui.labelTitle);
	ignoreWidget(m_ui.icon);
	createMenuBar();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUIFrame::createMenuBar()
{
	QMenuBar* mainMenu = new QMenuBar();
	m_fileMenu = new FileMenu(this);
	mainMenu->addMenu(m_fileMenu);
	m_layoutMenu = new LayoutMenu(this);
	mainMenu->addMenu(m_layoutMenu);
	mainMenu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_ui.layoutMenu->addWidget(mainMenu);
	mainMenu->setStyleSheet(menuBarStyle);
}
