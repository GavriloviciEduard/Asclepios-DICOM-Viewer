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
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initData()
{
	m_filesImporter = std::make_unique<FilesImporter>(this);
	m_filesImporter->startImporter();
	m_thumbnailsWidget = new ThumbnailsWidget(this);
	m_buttonsWidget = new ButtonsWidget(this);
	m_ui.toolBarButtons->addWidget(m_buttonsWidget);
	m_ui.toolBarThumbnails->addWidget(m_thumbnailsWidget);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::createConnections() const
{
	connectGUIActions();
	connectButtonsWidgetActions();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectGUIActions() const
{
	Q_UNUSED(connect(m_ui.actionOpenDICOMfile,
		&QAction::triggered, this, &asclepios::gui::GUI::onOpenFile));
	Q_UNUSED(connect(m_ui.actionOpenDICOMfolder,
		&QAction::triggered, this, &asclepios::gui::GUI::onOpenFolder))
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectButtonsWidgetActions() const
{
	Q_UNUSED(connect(m_buttonsWidget->getUI().buttonOpenFile,
		&QPushButton::pressed, this, &asclepios::gui::GUI::onOpenFile));
	Q_UNUSED(connect(m_buttonsWidget->getUI().buttonOpenFolder,
		&QPushButton::pressed, this, &asclepios::gui::GUI::onOpenFolder));
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
