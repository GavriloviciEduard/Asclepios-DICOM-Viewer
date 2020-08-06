#pragma once

#include <memory>
#include <vector>
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
		[[nodiscard]] int getIndex() const { return m_index; }
		[[nodiscard]] std::vector<std::unique_ptr<Study>>& getStudies() { return m_studies; }

		//setters
		void setID(const std::string& t_id) { m_id = t_id; }
		void setName(const std::string& t_fullName) { m_name = t_fullName; }
		void setAge(const int& t_age) { m_age = t_age; }
		void setBirthDate(const std::string& t_birthDate) { m_birthDate = t_birthDate; }
		void setIndex(const int& t_index) { m_index = t_index; }


		[[nodiscard]] Study* addStudy(std::unique_ptr<Study> t_study);

		//find
		[[nodiscard]] std::size_t findStudyIndex(Study* t_study);

	private:
		std::size_t m_index = -1;
		std::string m_id = {};
		std::string m_name = {};
		int m_age = {};
		std::string m_birthDate = {};
		std::vector<std::unique_ptr<Study>> m_studies = {};

	};
}
