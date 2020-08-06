#include "patient.h"
#include <algorithm>

asclepios::core::Study* asclepios::core::Patient::addStudy(std::unique_ptr<Study> t_study)
{
	auto index = findStudyIndex(t_study.get());
	if(index == m_studies.size())
	{
		m_studies.emplace_back(std::move(t_study));
		index = m_studies.size() - 1;
	}
	m_studies.at(index)->setIndex(index);
	return m_studies.at(index).get();
}

//-----------------------------------------------------------------------------
std::size_t asclepios::core::Patient::findStudyIndex(Study* t_study)
{
	const auto it = std::find_if(m_studies.begin(),
		m_studies.end(), [&t_study](const std::unique_ptr<Study>& study)
	{
		return t_study->getUID() == study->getUID();
	});
	return  std::distance(m_studies.begin(), it);
}
