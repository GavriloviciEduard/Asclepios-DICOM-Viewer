#include "study.h"

asclepios::core::Series* asclepios::core::Study::addSeries(std::unique_ptr<Series> t_series)
{
	t_series->setParentObject(this);
	return m_series.emplace(std::move(t_series)).first->get();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Study::operator==(const Study& t_rhs) const
{
	return getParentObject() == t_rhs.getParentObject() && getUID()
		== t_rhs.getUID() && getID() == t_rhs.getID() &&
		getDescription() == t_rhs.getDescription() && getDate()
		== t_rhs.getDate();
}
