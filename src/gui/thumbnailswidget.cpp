#include "thumbnailswidget.h"
#include "patienttab.h"

asclepios::gui::ThumbnailsWidget::ThumbnailsWidget(QWidget* parent)
	: QWidget(parent)
{
	initView();
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::initView()
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::initData()
{
	m_patientsTabs = new QTabWidget(this);
	m_patientsTabs->setTabPosition(QTabWidget::West);
	m_patientsTabs->setObjectName("ThumbnailsPatients");
	layout()->addWidget(m_patientsTabs);
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::addThumbnail(core::Patient* t_patient, core::Study* t_study,
                                                    core::Series* t_series, core::Image* t_image) const
{
	if (!tryInsertExistingItem(t_patient, t_study, t_series, t_image))
	{
		insertNewItem(t_patient, t_study, t_series, t_image);
	}
}

//-----------------------------------------------------------------------------
bool asclepios::gui::ThumbnailsWidget::tryInsertExistingItem(core::Patient* t_patient, core::Study* t_study,
                                                             core::Series* t_series, core::Image* t_image) const
{
	for (int i = 0; i < m_patientsTabs->count(); ++i)
	{
		auto* const patientTab =
			dynamic_cast<PatientTab*>(m_patientsTabs->widget(i));
		if (patientTab->getPatientID().toStdString() == t_patient->getID())
		{
			auto* studyTab =
				patientTab->getStudyTab(QString(t_study->getUID().c_str()));
			if (!studyTab)
			{
				studyTab = patientTab->addNewStudy(t_study);
			}
			studyTab->insertNewSeries(t_series, t_image);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::insertNewItem(core::Patient* t_patient, core::Study* t_study,
                                                     core::Series* t_series, core::Image* t_image) const
{
	auto* newPatientTab = new PatientTab(m_patientsTabs);
	newPatientTab->setPatientID(t_patient->getID().c_str());
	newPatientTab->setPatientName(QString::fromLatin1(t_patient->getName().c_str()));
	auto* newStudyTab = newPatientTab->addNewStudy(t_study);
	newStudyTab->insertNewSeries(t_series, t_image);
	m_patientsTabs->addTab(newPatientTab, newPatientTab->getPatientName());
}
