#pragma once

#include <vtkDICOMReader.h>
#include <vtkWeakPointer.h>
#include <vtkSmartPointer.h>
#include "utils.h"

namespace asclepios::core
{
	class Series;

	class Image
	{
	public:
		Image() = default;
		~Image() = default;

		//getters
		[[nodiscard]] export Series* getParentObject() const { return m_parent; }
		[[nodiscard]] export std::string getImagePath() const { return m_path; }
		[[nodiscard]] export std::string getSOPInstanceUID() const { return m_sopInstanceUid; }
		[[nodiscard]] export std::string getClassUID() const { return m_classUid; }
		[[nodiscard]] export std::string getFrameOfRefernceID() const { return m_frameOfReferenceId; }
		[[nodiscard]] export std::string getModality() const { return m_modality; }
		[[nodiscard]] export int getWindowCenter() const { return m_windowsCenter; }
		[[nodiscard]] export int getWindowWidth() const { return m_windowWidth; }
		[[nodiscard]] export int getRows() const { return m_rows; }
		[[nodiscard]] export int getColumns() const { return m_columns; }
		[[nodiscard]] export int getNumberOfFrames() const { return m_numberOfFrames; }
		[[nodiscard]] export double getSliceLocation() const { return m_sliceLocation; }
		[[nodiscard]] export int getAcquisitionNumber() const { return m_acquisitionNumber; }
		[[nodiscard]] export bool getIsMultiFrame() const { return m_isMultiframe; }
		[[nodiscard]] export int getIndex() const { return m_index; }
		[[nodiscard]] export int getPixelSpacingX() const { return m_pixelSpacingX; }
		[[nodiscard]] export int getPixelSpacingY() const { return m_pixelSpacingY; }
		
		/**
		* Getter for image reader. If image reader is null is created.
		*/
		[[nodiscard]] export vtkSmartPointer<vtkDICOMReader> getImageReader() const;

		//setters
		void export setParentObject(Series* t_parent) { m_parent = t_parent; }
		void export setImagePath(const std::string& t_path) { m_path = t_path; }
		void export setSOPInstanceUID(const std::string& t_sopInstanceUid) { m_sopInstanceUid = t_sopInstanceUid; }
		void export setClassUID(const std::string& t_classUid) { m_classUid = t_classUid; }
		void export setFrameOfRefernceID(const std::string& t_frameOfReferenceId) { m_frameOfReferenceId = t_frameOfReferenceId; }
		void export setModality(const std::string& t_modality) { m_modality = t_modality; }
		void export setWindowCenter(const int& t_windowsCenter) { m_windowsCenter = t_windowsCenter; }
		void export setWindowWidth(const int& t_windowWidth) { m_windowWidth = t_windowWidth; }
		void export setRows(const int& t_rows) { m_rows = t_rows; }
		void export setColumns(const int& t_columns) { m_columns = t_columns; }
		void export setNumberOfFrames(const int& t_numberOfFrames) { m_numberOfFrames = t_numberOfFrames; }
		void export setSliceLocation(const double& t_sliceLocation) { m_sliceLocation = t_sliceLocation; }
		void export setAcquisitionNumber(const int& t_acquisitionNumber) { m_acquisitionNumber = t_acquisitionNumber; }
		void export setIsMultiFrame(const bool& t_isMultiframe) { m_isMultiframe = t_isMultiframe; }
		void export setIndex(const int& t_index) { m_index = t_index; }
		void export setPixelSpacingX(const double& t_spacing) { m_pixelSpacingX = t_spacing; }
		void export setPixelSpacingY(const double& t_spacing) { m_pixelSpacingY = t_spacing; }
		
		/**
		* Functor for set compare
		*/
		struct imageCompare
		{
			bool operator()(const std::unique_ptr<Image>& t_lhs, const std::unique_ptr<Image>& t_rhs) const
			{
				return isLess(t_lhs.get(), t_rhs.get());
			}
		};

		export bool equal(Image* t_image) const;

	private:
		std::size_t m_index = -1;
		Series* m_parent = {};
		std::string m_path = {};
		std::string m_sopInstanceUid = {};
		std::string m_classUid = {};
		std::string m_frameOfReferenceId = {};
		std::string m_modality = {};
		int m_windowsCenter = {};
		int m_windowWidth = {};
		int m_rows = {};
		int m_columns = {};
		int m_numberOfFrames = {};
		double m_sliceLocation = {};
		double m_pixelSpacingX = -1;
		double m_pixelSpacingY = -1;
		int m_acquisitionNumber = {};
		bool m_isMultiframe = false;
		vtkWeakPointer<vtkDICOMReader> m_imageReader = {};

		static bool isLess(Image* t_lhs, Image* t_rhs);
	};
}
