#pragma once

#include <QFuture>
#include "patient.h"
#include "ui_studylist.h"
#include "seriesitem.h"


namespace asclepios::gui
{
	class StudyList final : public QListWidget
	{
	Q_OBJECT

	public:
		explicit StudyList(QWidget* parent = Q_NULLPTR);
		~StudyList() = default;

		//getters
		[[nodiscard]] core::Patient* getPatient() const { return m_patient; }
		[[nodiscard]] core::Study* getStudy() const { return m_study; }
		[[nodiscard]] std::vector<QFuture<void>> getFutures() const { return m_futures; }

		//setters
		void setPatient(core::Patient* t_patient) { m_patient = t_patient; }
		void setStudy(core::Study* t_study) { m_study = t_study; }

		void insertNewSeries(core::Series* t_series, core::Image* t_image);

	signals:
		void finishConcurrent();

	protected:
		void startDrag(Qt::DropActions supportedActions) override;

	private slots:
		void cleanUp();

	private:
		Ui::StudyList m_ui = {};
		core::Patient* m_patient = {};
		core::Study* m_study = {};
		std::vector<QFuture<void>> m_futures = {};

		void initView();
		[[nodiscard]] QString getDescription(core::Study* t_study, core::Series* t_series) const;
		[[nodiscard]] QString createMimeData(core::Series* t_series, core::Image* t_image);
		static void createImageForItem(StudyList* t_self, core::Image* t_image, SeriesItem* t_item);
	};
}
