#include "filesimporter.h"
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
	for (const auto& path : t_paths)
	{
		m_filesMutex.lock();
		m_filesPaths.push_back(path);
		m_filesMutex.unlock();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::FilesImporter::addFolders(const QStringList& t_paths)
{
	m_foldersMutex.lock();
	m_foldersPaths.append(t_paths);
	m_foldersMutex.unlock();
	m_futureFolders.waitForFinished();
	m_futureFolders = QtConcurrent::run(parseFolders, this);
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
void asclepios::gui::FilesImporter::importFiles()
{
	while (!m_filesPaths.empty() && m_isWorking)
	{
		m_filesMutex.lock();
		m_coreController->
			readData(m_filesPaths.front().toStdString());
		m_filesPaths.pop_front();
		m_filesMutex.unlock();
	}
	std::cout << m_coreController->getPatients().size();
}
