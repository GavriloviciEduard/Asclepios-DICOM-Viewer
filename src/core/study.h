#pragma once

#include <unordered_set>
#include <memory>
#include "series.h"

namespace asclepios::core
{
	class Study
	{
	public:
		Study() = default;
		~Study() = default;

		std::unordered_set<std::unique_ptr<Series>>& getSeries() { return m_series; }

	private:
		std::unordered_set<std::unique_ptr<Series>> m_series = {};
	};
}
