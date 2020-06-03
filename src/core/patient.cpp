#include "patient.h"


asclepios::core::Study* asclepios::core::Patient::addStudy(std::unique_ptr<Study> t_study)
{
	t_study->setParentObject(this);
	return m_studies.emplace(std::move(t_study)).first->get();
}

bool asclepios::core::Patient::isLess(Patient* t_lhs, Patient* t_rhs)
{
	return t_lhs->getAge() < t_rhs->getAge() && t_lhs->getBirthDate() <
		t_rhs->getBirthDate() && t_lhs->getID() < t_rhs->getID() &&
		t_lhs->getName() < t_rhs->getName();
}
