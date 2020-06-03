#pragma once

#include <memory>
#include "series.h"

namespace asclepios::core
{
	class Patient;

	class Study
	{
	public:
		Study() = default;
		~Study() = default;

		//getters
		[[nodiscard]] Patient* getParentObject() const { return m_parent; }
		[[nodiscard]] std::string getUID() const { return m_uid; }
		[[nodiscard]] std::string getID() const { return m_id; }
		[[nodiscard]] std::string getDescription() const { return m_desctiption; }
		[[nodiscard]] std::string getDate() const { return m_date; }
		std::set<std::unique_ptr<Series>, Series::seriesCompare>& getSeries() { return m_series; }

		//setters
		void setParentObject(Patient* t_parent) { m_parent = t_parent; }
		void setUID(const std::string& t_uid) { m_uid = t_uid; }
		void setID(const std::string& t_id) { m_id = t_id; }
		void setDescription(const std::string& t_description) { m_desctiption = t_description; }
		void setDate(const std::string& t_date) { m_date = t_date; }

		Series* addSeries(std::unique_ptr<Series> t_series);

		/**
		* Functor for set compare
		*/
		struct studyCompare
		{
			bool operator()(const std::unique_ptr<Study>& t_lhs, const std::unique_ptr<Study>& t_rhs) const
			{
				return isLess(t_lhs.get(), t_rhs.get());
			}
		};

	private:
		Patient* m_parent = {};
		std::string m_uid = {};
		std::string m_id = {};
		std::string m_desctiption = {};
		std::string m_date = {};
		std::set<std::unique_ptr<Series>, Series::seriesCompare> m_series = {};

		static bool isLess(Study* t_lhs, Study* t_rhs);
	};
}
