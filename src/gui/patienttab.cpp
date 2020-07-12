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
asclepios::gui::StudyList* asclepios::gui::PatientTab::getStudyTab(const QString& t_studyuid) const
{
	for (int i = 0; i < count(); i++)
	{
		auto* const item = dynamic_cast<StudyList*>(widget(i));
		if(item->getStudy()->getUID() == t_studyuid.toStdString())
		{
			return item;
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
asclepios::gui::StudyList* asclepios::gui::PatientTab::addNewStudy(core::Study* t_study)
{
	auto* list = new StudyList(this);
	list->setViewMode(QListWidget::IconMode);
	list->setIconSize(QSize(150, 150));
	list->setPatient(t_study->getParentObject());
	list->setStudy(t_study);
	addTab(list, QString::fromLatin1(t_study->getDescription().c_str()));
	return list;
}
