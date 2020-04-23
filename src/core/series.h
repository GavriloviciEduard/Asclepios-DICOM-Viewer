#pragma once

#include <set>
#include <memory>
#include "image.h"

namespace asclepios::core
{
	class Study;

	class Series
	{
	public:
		Series() = default;
		~Series() = default;

		//getters
		[[nodiscard]] Study* getParentObject() const { return m_parent; }
		[[nodiscard]] std::string getUID() const { return m_uid; }
		[[nodiscard]] std::string getDescription() const { return m_desctiption; }
		[[nodiscard]] std::string getDate() const { return m_date; }
		[[nodiscard]] std::string getNumber() const { return m_number; }
		std::set<std::unique_ptr<Image>>& getSinlgeFrameImages() { return m_singleFrameImages; }
		std::set<std::unique_ptr<Image>>& getMultiFrameImages() { return m_multiFrameImages; }

		//setters
		void setParentObject(Study* t_parent) { m_parent = t_parent; }
		void setUID(const std::string& t_uid) { m_uid = t_uid; }
		void setDescription(const std::string& t_description) { m_desctiption = t_description; }
		void setDate(const std::string& t_date) { m_date = t_date; }
		void setNumber(const std::string& t_number) { m_number = t_number; }

		void addSingleFrameImage(std::unique_ptr<Image> t_image);
		void addMultiFrameImage(std::unique_ptr<Image> t_image);
		bool operator==(const Series& t_rhs) const;

	private:
		Study* m_parent = {};
		std::string m_uid = {};
		std::string m_desctiption = {};
		std::string m_date = {};
		std::string m_number = {};
		std::set<std::unique_ptr<Image>> m_singleFrameImages = {};
		std::set<std::unique_ptr<Image>> m_multiFrameImages = {};
	};
}
