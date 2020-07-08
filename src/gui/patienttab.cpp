#include "patienttab.h"

asclepios::gui::PatientTab::PatientTab(QWidget *parent)
	: QTabWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::PatientTab::initView()
{
	m_ui.setupUi(this);
	setTabPosition(West);
}

//-----------------------------------------------------------------------------
asclepios::gui::StudyList* asclepios::gui::PatientTab::getStudyItem(const QString& t_studyuid) const
{
	//todo
	return nullptr;
}

//-----------------------------------------------------------------------------
asclepios::gui::StudyList* asclepios::gui::PatientTab::addNewStudy(core::Study* t_study)
{
	//todo
	return nullptr;
}
