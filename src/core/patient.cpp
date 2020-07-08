#include "patient.h"


asclepios::core::Study* asclepios::core::Patient::addStudy(std::unique_ptr<Study> t_study)
{
	return m_studies.emplace(std::move(t_study)).first->get();
}

bool asclepios::core::Patient::isLess(Patient* t_lhs, Patient* t_rhs)
{
	return t_lhs->getID() < t_rhs->getID();
}
