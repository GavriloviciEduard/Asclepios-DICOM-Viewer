#include "gui.h"
#include <QWindow>
#include <QPainter>
#include <QTimer>
#include <QPushButton>
#include <QFileDialog>

asclepios::gui::GUI::GUI(QWidget* parent) : QMainWindow(parent, Qt::FramelessWindowHint)
{
	initView();
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initView()
{
	m_ui.setupUi(this);
	setUpFramelessHelper();
	showMaximized();
	createConnections();
	m_ui.maximizeButton->setIcon(QIcon(buttonMaximizeOn));
	m_ui.icon->setPixmap(QPixmap::fromImage(QImage(iconTitleBar)));
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initData()
{
	m_coreController = std::make_unique<core::CoreController>();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::openFileClicked()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
	if (!fileName.isEmpty())
	{
		//open should be async
		//todo make async class for opening files
		m_coreController->readData(fileName.toStdString());
	}
	else
	{
		//todo make alert class
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::openFolderClicked()
{
}


//-----------------------------------------------------------------------------
void asclepios::gui::GUI::updateMaximizeButton(const bool& maximized) const
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
void asclepios::gui::GUI::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	const QImage backgroundImage(appBackground);
	painter.drawImage(contentsRect(), backgroundImage);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::setUpFramelessHelper()
{
	m_helper = std::make_unique<FramelessHelper>(this);
	m_helper->setDraggableMargins(3, 3, 3, 3);
	m_helper->setMaximizedMargins(3, 3, 3, 3);
	m_helper->setTitleBarHeight(32);
	m_helper->addExcludeItem(m_ui.minimizeButton);
	m_helper->addExcludeItem(m_ui.maximizeButton);
	m_helper->addExcludeItem(m_ui.closeButton);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::createConnections() const
{
	connect(m_ui.minimizeButton, &QPushButton::clicked,
	        m_helper.get(), &FramelessHelper::triggerMinimizeButtonAction);
	connect(m_ui.maximizeButton, &QPushButton::clicked,
	        m_helper.get(), &FramelessHelper::triggerMaximizeButtonAction);
	connect(m_ui.closeButton, &QPushButton::clicked,
	        m_helper.get(), &FramelessHelper::triggerCloseButtonAction);
	connect(m_helper.get(), &FramelessHelper::maximizedChanged,
	        this, &GUI::updateMaximizeButton);
}
