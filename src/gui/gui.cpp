#include "gui.h"
#include <QFileDialog>

asclepios::gui::GUI::GUI(QWidget* parent) : QMainWindow(parent)
{
	initView();
	initData();
	createConnections();
}

asclepios::gui::GUI::~GUI()
{
	if (m_filesImporter->isRunning())
	{
		m_filesImporter->stopImporter();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initView()
{
	m_ui.setupUi(this);
	setAutoFillBackground(false);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initData()
{
	//create widget controller
	m_widgetsController = std::make_unique<WidgetsController>();
	m_widgetsController->createWidgets(WidgetsContainer::layouts::twoRowOneBottom);
	setCentralWidget(m_widgetsController->getWidgetsContainer());

	//create file importer
	m_filesImporter = std::make_unique<FilesImporter>(this);
	m_filesImporter->startImporter();

	//create toolbars and widgets
	m_thumbnailsWidget = new ThumbnailsWidget(this);
	m_imageFunctionsTtoolbar = new ImageFunctionsToolbar(this);
	m_ui.toolBarImageTransf->addWidget(m_imageFunctionsTtoolbar);
	m_generalToolbar = new GeneralToolbar(this);
	m_ui.toolBarButtons->addWidget(m_generalToolbar);
	createLayoutMenu();
	m_ui.toolBarThumbnails->addWidget(m_thumbnailsWidget);
}

void asclepios::gui::GUI::createLayoutMenu()
{
	m_layoutMenu = new LayoutMenu(this);
	auto* const toolBt = m_generalToolbar->getUI().buttonLayout;
	toolBt->setMenu(m_layoutMenu);
	toolBt->setPopupMode(QToolButton::InstantPopup);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::createConnections() const
{
	connectGUIActions();
	connectGeneralToolbar();
	connectFilesImporter();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectGUIActions() const
{
	Q_UNUSED(connect(m_ui.actionOpenDICOMfile,
		&QAction::triggered, this, &asclepios::gui::GUI::onOpenFile));
	Q_UNUSED(connect(m_ui.actionOpenDICOMfolder,
		&QAction::triggered, this, &asclepios::gui::GUI::onOpenFolder))
	//todo connect mpr and volume to viewers
	//todo connect image functions to viewers
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectGeneralToolbar() const
{
	Q_UNUSED(connect(m_generalToolbar->getUI().buttonOpenFile,
		&QPushButton::pressed, this, &asclepios::gui::GUI::onOpenFile));
	Q_UNUSED(connect(m_generalToolbar->getUI().buttonOpenFolder,
		&QPushButton::pressed, this, &asclepios::gui::GUI::onOpenFolder));
	//todo connect mpr and volume to viewers
	//todo connect image functions to viewers
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectFilesImporter() const
{
	Q_UNUSED(connect(m_filesImporter.get(),
		&FilesImporter::addNewThumbnail,
		m_thumbnailsWidget, &ThumbnailsWidget::addThumbnail));
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::onOpenFile()
{
	auto* const fileDialog =
		new QFileDialog(this, "Select files");
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		m_filesImporter->addFiles(fileDialog->selectedFiles());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::onOpenFolder()
{
	auto* const filedialog =
		new QFileDialog(this, "Select folder");
	filedialog->setFileMode(QFileDialog::Directory);
	filedialog->setOptions(QFileDialog::ShowDirsOnly);
	if (filedialog->exec() == QDialog::Accepted)
	{
		m_filesImporter->addFolders(filedialog->selectedFiles());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::onChangeLayout(const WidgetsContainer::layouts& t_layout) const
{
	m_widgetsController->createWidgets(t_layout);
	QApplication::restoreOverrideCursor();
}
