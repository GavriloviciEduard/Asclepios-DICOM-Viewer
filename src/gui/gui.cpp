#include "gui.h"
#include <QWindow>
#include <QScreen>
#include <QPainter>
#include <QTimer>
#include <QPushButton>

asclepios::gui::GUI::GUI(QWidget* parent) : QMainWindow(parent, Qt::FramelessWindowHint)
{
	//m_coreController = std::make_unique<core::CoreController>();
	//m_coreController->readData(R"(C:\Users\GavEd\Desktop\cucu\CucuMpr\cucu mpr\08_1.25 mm\001_1531206301965 _CUCU TOADER .dcm)");
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initView()
{
	m_ui.setupUi(this);
	setUpFramelessHelper();
	createConnections();
	m_ui.maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button1.png")));
	m_ui.labelTitle->setStyleSheet("font-weight: bold; color: grey");
	m_ui.icon->setPixmap(QPixmap::fromImage(QImage(":/res/icon_small.png")));
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::updateMaximizeButton(const bool& maximized) const
{
	if (maximized)
	{
		m_ui.maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button2.png")));
		m_ui.maximizeButton->setToolTip(tr("Restore"));
	}
	else
	{
		m_ui.maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button1.png")));
		m_ui.maximizeButton->setToolTip(tr("Maximize"));
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	const QImage backgroundImage(QStringLiteral(":/res/background.png"));
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
