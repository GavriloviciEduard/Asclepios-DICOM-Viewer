#include "study.h"

asclepios::core::Series* asclepios::core::Study::addSeries(std::unique_ptr<Series> t_series)
{
	t_series->setParentObject(this);
	return m_series.emplace(std::move(t_series)).first->get();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Study::isLess(Study* t_lhs, Study* t_rhs)
{
	return t_lhs->getParentObject() == t_rhs->getParentObject()
		&& t_lhs->getUID() == t_rhs->getUID() && t_lhs->getID()
		== t_rhs->getID() && t_lhs->getDescription() ==
		t_rhs->getDescription() && t_lhs->getDate() == t_rhs->getDate();
}
