#pragma once

#include <deque>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include <qmutex.h>
#include <QThread>
#include "corecontroller.h"

namespace asclepios::gui
{
	class FilesImporter final : public QThread
	{
	Q_OBJECT

	public:
		explicit FilesImporter(QObject* parent) : QThread(Q_NULLPTR),
			m_coreController(std::make_unique<core::CoreController>()) {}
		~FilesImporter() = default;

		void startImporter();
		void stopImporter();
		void addFiles(const QStringList& t_paths);
		void addFolders(const QStringList& t_paths);
		core::CoreController* getCoreController() const { return m_coreController.get(); }

	signals:
		void addNewThumbnail(core::Patient* t_patient,
			core::Study* t_study, core::Series* t_series, core::Image* t_image);
		void populateWidget(core::Series* t_series, core::Image* t_image,
			const int& t_patientIndex, const int& t_studyIndex,
			const int& t_seriesIndex, const int& t_imageIndex);
		void refreshSliderValues(const int& t_patientIndex, const int& t_studyIndex,
			const int& t_seriesIndex, const int& t_imageIndex, const int& t_seriesSize);

	protected:
		void run() override;

	private slots:
		void parseFoldersFinished() const;

	private:
		QMutex m_filesMutex;
		QMutex m_foldersMutex;
		QFuture<void> m_futureFolders;
		QFutureWatcher<void> m_futureWatcherFolders;
		std::unique_ptr<core::CoreController> m_coreController = {};
		std::deque<QString> m_filesPaths;
		QStringList m_foldersPaths;
		bool m_isWorking = false;

		void importFiles();
		static void parseFolders(FilesImporter* t_self);
	};
}
