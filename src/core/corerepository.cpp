#include "corerepository.h"
#include <algorithm>


void asclepios::core::CoreRepository::addPatient(std::unique_ptr<Patient> t_patient)
{
	auto index = findPatient(t_patient.get());
	if (index == m_patients.size())
	{
		m_patients.emplace_back(std::move(t_patient));
		index = m_patients.size() - 1;
	}
	m_lastPatient = m_patients.at(index).get();
	m_lastPatient->setIndex(index);
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
	m_lastSeries = m_lastStudy->addSeries(std::move(t_series), m_newSeries);
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::addImage(std::unique_ptr<Image> t_image)
{
	if (!t_image)
	{
		return;
	}
	t_image->setParentObject(m_lastSeries);
	m_lastImage = t_image->getIsMultiFrame()
		? m_lastSeries->addMultiFrameImage(std::move(t_image), m_newImage)
		: m_lastSeries->addSingleFrameImage(std::move(t_image), m_newImage);
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreRepository::deletePatient(Patient* t_patient)
{
	const auto index = findPatient(t_patient);
	if (index != m_patients.size())
	{
		m_patients.erase(m_patients.begin() + index);
	}
}

//-----------------------------------------------------------------------------
std::size_t asclepios::core::CoreRepository::findPatient(Patient* t_patient)
{
	const auto it = std::find_if(m_patients.begin(),
		m_patients.end(), [&t_patient](const std::unique_ptr<Patient>& patient)
	{
		return t_patient->getID() == patient->getID();
	});
	return  std::distance(m_patients.begin(), it);
}

