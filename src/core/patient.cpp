#include "patient.h"


asclepios::core::Study* asclepios::core::Patient::addStudy(std::unique_ptr<Study> t_study)
{
	t_study->setParentObject(this);
	return m_studies.emplace(std::move(t_study)).first->get();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Patient::operator==(const Patient& t_rhs) const
{
	return getAge() == t_rhs.getAge() && getBirthDate() ==
		t_rhs.getBirthDate() && getID() == t_rhs.getID() &&
		getName() == t_rhs.getName();
}