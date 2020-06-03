#include "corerepository.h"
#include <algorithm>


void asclepios::core::CoreRepository::addPatient(std::unique_ptr<Patient> t_patient)
{
	m_lastPatient = m_patients.emplace(std::move(t_patient)).first->get();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::addStudy(std::unique_ptr<Study> t_study)
{
	t_study->setParentObject(m_lastPatient);
	m_lastStudy = m_lastPatient->addStudy(std::move(t_study));
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::addSeries(std::unique_ptr<Series> t_series)
{
	t_series->setParentObject(m_lastStudy);
	m_lastSeries = m_lastStudy->addSeries(std::move(t_series));
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::addImage(std::unique_ptr<Image> t_image) const
{
	if (!t_image)
	{
		return;
	}
	t_image->setParentObject(m_lastSeries);
	t_image->getIsMultiFrame()
		? m_lastSeries->addMultiFrameImage(std::move(t_image))
		: m_lastSeries->addSingleFrameImage(std::move(t_image));
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::deletePatient(Patient* t_patient)
{
	const auto it = std::find_if(m_patients.begin(),
	m_patients.end(), [&t_patient](const std::unique_ptr<Patient>& patient)
	{
		return t_patient == patient.get();
	});
	if (it != m_patients.end())
	{
		m_patients.erase(it);
	}
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::deleteAllPatients()
{
	m_patients.clear();
}
