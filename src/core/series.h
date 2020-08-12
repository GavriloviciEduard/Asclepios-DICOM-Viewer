#pragma once

#include <set>
#include <memory>
#include "image.h"
#include "utils.h"

namespace asclepios::core
{
	class Study;

	class Series
	{
	public:
		Series() = default;
		~Series() = default;

		//getters
		[[nodiscard]] export Study* getParentObject() const { return m_parent; }
		[[nodiscard]] export std::string getUID() const { return m_uid; }
		[[nodiscard]] export std::string getDescription() const { return m_desctiption; }
		[[nodiscard]] export std::string getDate() const { return m_date; }
		[[nodiscard]] export std::string getNumber() const { return m_number; }
		[[nodiscard]] export Image* getNextSingleFrameImage(Image* t_image);
		[[nodiscard]] export Image* getPreviousSingleFrameImage(Image* t_image);
		[[nodiscard]] export Image* getSingleFrameImageByIndex(const int& t_index);
		[[nodiscard]] export std::set<std::unique_ptr<Image>, Image::imageCompare>& getSinlgeFrameImages() { return m_singleFrameImages; }
		[[nodiscard]] export std::set<std::unique_ptr<Image>, Image::imageCompare>& getMultiFrameImages() { return m_multiFrameImages; }
		[[nodiscard]] export int getIndex() const { return m_index; }
		[[nodiscard]] export vtkSmartPointer<vtkDICOMReader> getReaderForAllSingleFrameImages();
		
		//setters
		export void setParentObject(Study* t_parent) { m_parent = t_parent; }
		export void setUID(const std::string& t_uid) { m_uid = t_uid; }
		export void setDescription(const std::string& t_description) { m_desctiption = t_description; }
		export void setDate(const std::string& t_date) { m_date = t_date; }
		export void setNumber(const std::string& t_number) { m_number = t_number; }
		export void setIndex(const int& t_index) { m_index = t_index; }
		
		[[nodiscard]] export Image* addSingleFrameImage(std::unique_ptr<Image> t_image, bool& t_newImage);
		[[nodiscard]] export Image* addMultiFrameImage(std::unique_ptr<Image> t_image, bool& t_newImage);
		[[nodiscard]] export std::size_t findImageIndex(const std::set<std::unique_ptr<Image>, Image::imageCompare>& t_images,
			Image* t_image);

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
		std::size_t m_index = -1;
		Study* m_parent = {};
		std::string m_uid = {};
		std::string m_desctiption = {};
		std::string m_date = {};
		std::string m_number = {};
		vtkWeakPointer<vtkDICOMReader> m_readerSingleFrame = {};
		std::set<std::unique_ptr<Image>, Image::imageCompare> m_singleFrameImages = {};
		std::set<std::unique_ptr<Image>, Image::imageCompare> m_multiFrameImages = {};

		static bool isLess(Series* t_lhs, Series* t_rhs);
		
	};
}
