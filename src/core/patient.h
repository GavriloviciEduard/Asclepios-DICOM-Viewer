#pragma once

#include <memory>
#include "study.h"

namespace asclepios::core
{
	class Patient
	{
	public:
		Patient() = default;
		~Patient() = default;

		//getters
		[[nodiscard]] std::string getID() const { return m_id; }
		[[nodiscard]] std::string getName() const { return m_name; }
		[[nodiscard]] int getAge() const { return m_age; }
		[[nodiscard]] std::string getBirthDate() const { return m_birthDate; }
		[[nodiscard]] std::set<std::unique_ptr<Study>>& getStudies() { return m_studies; }

		//setters
		void setID(const std::string& t_id) { m_id = t_id; }
		void setName(const std::string& t_fullName) { m_name = t_fullName; }
		void setAge(const int& t_age) { m_age = t_age; }
		void setBirthDate(const std::string& t_birthDate) { m_birthDate = t_birthDate; }


		[[nodiscard]] Study* addStudy(std::unique_ptr<Study> t_study);
		static bool isLess(Patient* t_lhs, Patient* t_rhs);

		//functor for set compare
		struct patientCompare
		{
			bool operator()(const std::unique_ptr<Patient>& t_lhs, const std::unique_ptr<Patient>& t_rhs) const
			{
				return isLess(t_lhs.get(), t_rhs.get());
			}
		};

	private:
		std::string m_id = {};
		std::string m_name = {};
		int m_age = {};
		std::string m_birthDate = {};
		std::set<std::unique_ptr<Study>> m_studies = {};
	};
}
