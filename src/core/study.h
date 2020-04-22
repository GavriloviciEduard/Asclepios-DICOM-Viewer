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

		//getters
		[[nodiscard]] std::string getUID() const { return m_uid; }
		[[nodiscard]] std::string getID() const { return m_id; }
		[[nodiscard]] std::string getDescription() const { return m_desctiption; }
		[[nodiscard]] std::string getDate() const { return m_date; }
		std::unordered_set<std::unique_ptr<Series>>& getSeries() { return m_series; }

		//setters
		void setUID(const std::string& t_uid) { m_uid = t_uid; }
		void setID(const std::string& t_id) { m_id = t_id; }
		void setDescription(const std::string& t_description) { m_desctiption = t_description; }
		void setDate(const std::string& t_date) { m_date = t_date; }

	private:
		std::string m_uid = {};
		std::string m_id = {};
		std::string m_desctiption = {};
		std::string m_date = {};
		std::unordered_set<std::unique_ptr<Series>> m_series = {};
	};
}
