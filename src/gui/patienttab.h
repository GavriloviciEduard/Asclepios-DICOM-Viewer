#pragma once

#include <QTabWidget>
#include "studylist.h"
#include "ui_patienttab.h"

namespace asclepios::core
{
	class Study;
}

namespace asclepios::gui
{
	class PatientTab final : public QTabWidget
	{
	Q_OBJECT

	public:
		explicit PatientTab(QWidget* parent = Q_NULLPTR);
		~PatientTab() = default;

		//getters
		[[nodiscard]] QString getPatientName() const { return m_patientName; }
		[[nodiscard]] QString getPatientID() const { return m_patientid; }
		[[nodiscard]] StudyList* getStudyTab (const QString& t_studyuid) const;

		//setters
		void setPatientName(const QString& t_patientName) { m_patientName = t_patientName; }
		void setPatientID(const QString& t_patientid) { m_patientid = t_patientid; }

		StudyList* addNewStudy(core::Study* t_study);


	private:
		Ui::PatientTab m_ui = {};
		QString m_patientName = {};
		QString m_patientid = {};

		void initView();
	};
}
