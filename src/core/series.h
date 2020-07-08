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
		std::set<std::unique_ptr<Image>, Image::imageCompare>& getSinlgeFrameImages() { return m_singleFrameImages; }
		std::set<std::unique_ptr<Image>, Image::imageCompare>& getMultiFrameImages() { return m_multiFrameImages; }

		//setters
		void setParentObject(Study* t_parent) { m_parent = t_parent; }
		void setUID(const std::string& t_uid) { m_uid = t_uid; }
		void setDescription(const std::string& t_description) { m_desctiption = t_description; }
		void setDate(const std::string& t_date) { m_date = t_date; }
		void setNumber(const std::string& t_number) { m_number = t_number; }

		void addSingleFrameImage(std::unique_ptr<Image> t_image);
		void addMultiFrameImage(std::unique_ptr<Image> t_image);

		/**
		* Functor for set compare
		*/
		struct seriesCompare
		{
			bool operator()(const std::unique_ptr<Series>& t_lhs, const std::unique_ptr<Series>& t_rhs) const
			{
				return isLess(t_lhs.get(), t_rhs.get());
			}
		};

	private:
		Study* m_parent = {};
		std::string m_uid = {};
		std::string m_desctiption = {};
		std::string m_date = {};
		std::string m_number = {};
		std::set<std::unique_ptr<Image>, Image::imageCompare> m_singleFrameImages = {};
		std::set<std::unique_ptr<Image>, Image::imageCompare> m_multiFrameImages = {};

		static bool isLess(Series* t_lhs, Series* t_rhs);
		static bool imageAlreadyInserted(const std::set<std::unique_ptr<Image>, Image::imageCompare>& t_images,
		                                 Image* t_image);
	};
}
