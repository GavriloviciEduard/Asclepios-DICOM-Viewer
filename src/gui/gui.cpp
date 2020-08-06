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
	m_widgetsController->waitForRenderingThreads();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initView()
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::initData()
{
	//create file importer
	m_filesImporter =
		std::make_shared<FilesImporter>(this);
	m_filesImporter->startImporter();
	//create widget controller
	m_widgetsController =
		std::make_unique<WidgetsController>();
	m_widgetsController
		->createWidgets(WidgetsContainer::layouts::twoRowOneBottom);
	m_widgetsController
		->setFilesImporter(m_filesImporter.get());
	auto* const widgetsContainer
		= m_widgetsController->getWidgetsContainer();
	setCentralWidget(widgetsContainer);
	//create toolbars and widgets
	m_thumbnailsWidget = new ThumbnailsWidget();
	onShowThumbnailsWidget(false);
	widgetsContainer->getUI().widgetPatients
	                ->layout()->addWidget(m_thumbnailsWidget);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::createConnections() const
{
	connectFilesImporter();
	connectFunctions();
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectFilesImporter() const
{
	Q_UNUSED(connect(m_filesImporter.get(),
		&FilesImporter::addNewThumbnail,
		m_thumbnailsWidget,
		&ThumbnailsWidget::addThumbnail));
	Q_UNUSED(connect(m_filesImporter.get(),
		&FilesImporter::populateWidget,
		m_widgetsController.get(),
		&WidgetsController::populateWidget));
	Q_UNUSED(connect(m_filesImporter.get(),
		&FilesImporter::showThumbnailsWidget,
		this, &GUI::onShowThumbnailsWidget));
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::disconnectFilesImporter() const
{
	disconnect(m_filesImporter.get(),
	           &FilesImporter::addNewThumbnail,
	           m_thumbnailsWidget,
	           &ThumbnailsWidget::addThumbnail);
	disconnect(m_filesImporter.get(),
	           &FilesImporter::populateWidget,
	           m_widgetsController.get(),
	           &WidgetsController::populateWidget);
	disconnect(m_filesImporter.get(),
	           &FilesImporter::showThumbnailsWidget,
	           this, &GUI::onShowThumbnailsWidget);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::connectFunctions() const
{
	auto* const widgetsContainer =
		m_widgetsController->getWidgetsContainer();
	Q_UNUSED(connect(widgetsContainer,
		&WidgetsContainer::applyTransformation,
		this, &GUI::onApplyTransformation));
	Q_UNUSED(connect(widgetsContainer,
		&WidgetsContainer::closePatients,
		this, &GUI::onCloseAllPatients));
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

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::onApplyTransformation(const transformationType& t_type) const
{
	m_widgetsController->applyTransformation(t_type);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::onShowThumbnailsWidget(const bool& t_flag) const
{
	m_widgetsController->getWidgetsContainer()
		->getUI().widgetPatients->setVisible(t_flag);
}

//-----------------------------------------------------------------------------
void asclepios::gui::GUI::onCloseAllPatients() const
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	disconnectFilesImporter();
	onShowThumbnailsWidget(false);
	m_filesImporter->stopImporter();
	m_widgetsController->resetData();
	m_thumbnailsWidget->resetData();
	m_filesImporter->getCoreController()->resetData();
	connectFilesImporter();
	m_filesImporter->startImporter();
	QApplication::restoreOverrideCursor();
}
