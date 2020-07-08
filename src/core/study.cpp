#include "study.h"

asclepios::core::Series* asclepios::core::Study::addSeries(std::unique_ptr<Series> t_series)
{
	return m_series.emplace(std::move(t_series)).first->get();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Study::isLess(Study* t_lhs, Study* t_rhs)
{
	return t_lhs->getUID() < t_rhs->getUID();
}
