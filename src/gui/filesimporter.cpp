#include "filesimporter.h"
#include <QApplication>
#include <QtConcurrent/QtConcurrent>

void asclepios::gui::FilesImporter::startImporter()
{
	m_isWorking = true;
	start();
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::stopImporter()
{
	m_futureFolders.waitForFinished();
	m_isWorking = false;
	quit();
	wait();
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::addFiles(const QStringList& t_paths)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	for (const auto& path : t_paths)
	{
		m_filesMutex.lock();
		m_filesPaths.push_back(path);
		m_filesMutex.unlock();
	}
	QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::addFolders(const QStringList& t_paths)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	m_foldersMutex.lock();
	m_foldersPaths.append(t_paths);
	m_foldersMutex.unlock();
	m_futureFolders.waitForFinished();
	m_futureFolders = QtConcurrent::run(parseFolders, this);
	Q_UNUSED(connect(&m_futureWatcherFolders,
		&QFutureWatcher<void>::finished, this,
		&FilesImporter::parseFoldersFinished));
	m_futureWatcherFolders.setFuture(m_futureFolders);
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::parseFolders(FilesImporter* t_self)
{
	while (!t_self->m_foldersPaths.isEmpty())
	{
		t_self->m_foldersMutex.lock();
		QString folderPath = t_self->m_foldersPaths.front();
		QDirIterator it(folderPath, QDir::Files,
		                QDirIterator::Subdirectories);
		t_self->m_foldersMutex.unlock();
		while (it.hasNext())
		{
			it.next();
			t_self->m_filesMutex.lock();
			t_self->m_filesPaths.push_back(it.filePath());
			t_self->m_filesMutex.unlock();
		}
		t_self->m_foldersMutex.lock();
		t_self->m_foldersPaths.pop_front();
		t_self->m_foldersMutex.unlock();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::run()
{
	while (m_isWorking)
	{
		importFiles();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::parseFoldersFinished() const
{
	disconnect(&m_futureWatcherFolders,
		&QFutureWatcher<void>::finished, this,
		&FilesImporter::parseFoldersFinished);
	QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::importFiles()
{
	while (!m_filesPaths.empty() && m_isWorking)
	{
		m_filesMutex.lock();
		m_coreController->
			readData(m_filesPaths.front().toStdString());
		if (m_coreController->newSeriesAdded() ||
			m_coreController->newImageAdded() &&
			m_coreController->getLastImage()->getIsMultiFrame())
		{
			emit addNewThumbnail(m_coreController->getLastPatient(),
			                             m_coreController->getLastStudy(),
			                             m_coreController->getLastSeries(),
			                             m_coreController->getLastImage());
		}
		m_filesPaths.pop_front();
		m_filesMutex.unlock();
	}
}
