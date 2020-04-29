#include "gui.h"
#include <QWindow>

asclepios::gui::GUI::GUI(QWidget* parent) : QMainWindow(parent)
{
	initView();
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initView()
{
	m_ui.setupUi(this);
	createConnections();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initData()
{
	m_coreController = std::make_unique<core::CoreController>();
	m_thumbnailsWidget = std::make_unique<ThumbnailsWidget>();
	m_buttonsWidget = std::make_unique<ButtonsWidget>();
	m_ui.toolBarButtons->addWidget(m_buttonsWidget.get());
	m_ui.toolBarThumbnails->addWidget(m_thumbnailsWidget.get());
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::createConnections() const
{
	//todo
}


////-----------------------------------------------------------------------------
//void asclepios::gui::GUI::initLayoutMenu()
//{
//	m_layoutMenu = std::make_unique<LayoutMenu>(this);
//	m_ui.layoutButton->setMenu(m_layoutMenu.get());
//	m_ui.layoutButton->setPopupMode(QToolButton::InstantPopup);
//}

////-----------------------------------------------------------------------------
//void asclepios::gui::GUI::openFileClicked()
//{
//	const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
//	if (!fileName.isEmpty())
//	{
//		//open should be async
//		//todo make async class for opening files
//		m_coreController->readData(fileName.toStdString());
//		std::cout << m_coreController->getPatients().size() << '\n';
//	}
//	else
//	{
//		//todo make alert class
//	}
//}
