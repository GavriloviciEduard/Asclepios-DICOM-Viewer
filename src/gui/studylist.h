#pragma once

#include <QFuture>
#include <QListWidgetItem>
#include "patient.h"
#include "ui_studylist.h"

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

		void insertNewSeries(core::Series* t_series);

	private:
		Ui::StudyList m_ui = {};
		core::Patient* m_patient = {};
		core::Study* m_study = {};
		std::vector<QFuture<void>> m_futures = {};

		void initView();
	};
}
