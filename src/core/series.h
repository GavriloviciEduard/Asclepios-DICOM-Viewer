#pragma once

#include <unordered_set>
#include <memory>
#include "image.h"

namespace asclepios::core
{
	class Series
	{
	public:
		Series() = default;
		~Series() = default;

		//getters
		[[nodiscard]] std::string getUID() const { return m_uid; }
		[[nodiscard]] std::string getDescription() const { return m_desctiption; }
		[[nodiscard]] std::string getDate() const { return m_date; }
		[[nodiscard]] std::string getNumber() const { return m_number; }
		std::unordered_set<std::unique_ptr<Image>>& getImages() { return m_images; }

		//setters
		void setUID(const std::string& t_uid) { m_uid = t_uid; }
		void setDescription(const std::string& t_description) { m_desctiption = t_description; }
		void setDate(const std::string& t_date) { m_date = t_date; }
		void setNumber(const std::string& t_number) { m_number = t_number; }

	private:
		std::string m_uid = {};
		std::string m_desctiption = {};
		std::string m_date = {};
		std::string m_number = {};
		std::unordered_set<std::unique_ptr<Image>> m_images = {};
	};
}
