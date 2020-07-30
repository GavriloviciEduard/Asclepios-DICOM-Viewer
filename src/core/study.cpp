#include "study.h"
#include <algorithm>

asclepios::core::Series* asclepios::core::Study::addSeries(std::unique_ptr<Series> t_series, bool& t_newSeries)
{
	auto index = findSeriesIndex(t_series.get());
	t_newSeries = false;
	if (index == m_series.size())
	{
		m_series.emplace_back(std::move(t_series));
		index = m_series.size() - 1;
		t_newSeries = true;
	}
	m_series.at(index)->setIndex(index);
	return m_series.at(index).get();
}

//-----------------------------------------------------------------------------
std::size_t asclepios::core::Study::findSeriesIndex(Series* t_series)
{
	const auto it = std::find_if(m_series.begin(),
		m_series.end(), [&t_series](const std::unique_ptr<Series>& series)
	{
		return t_series->getUID() == series->getUID();
	});
	return  std::distance(m_series.begin(), it);
}
