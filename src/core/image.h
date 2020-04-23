#pragma once

#include <imebra.h>

namespace asclepios::core
{
	class Series;
	
	class Image
	{
	public:
		Image() = default;
		~Image() = default;

		//getters
		[[nodiscard]] Series* getParentObject() const { return m_parent; }
		[[nodiscard]] std::string getImagePath() const { return m_path; }
		[[nodiscard]] std::string getSOPInstanceUID() const { return m_sopInstanceUid; }
		[[nodiscard]] std::string getClassUID() const { return m_classUid; }
		[[nodiscard]] std::string getFrameOfRefernceID() const { return m_frameOfReferenceId; }
		[[nodiscard]] std::string getModality() const { return m_modality; }
		[[nodiscard]] int getWindowCenter() const { return m_windowsCenter; }
		[[nodiscard]] int getWindowWidth() const { return m_windowWidth; }
		[[nodiscard]] int getRows() const { return m_rows; }
		[[nodiscard]] int getColumns() const { return m_columns; }
		[[nodiscard]] int getNumberOfFrames() const { return m_numberOfFrames; }
		[[nodiscard]] double getSliceLocation() const { return m_sliceLocation; }
		[[nodiscard]] bool getIsMultiFrame() const { return m_isMultiframe; }
		
		//setters
		void setParentObject(Series* t_parent) { m_parent = t_parent; }
		void setImagePath(const std::string& t_path) { m_path = t_path; }
		void setSOPInstanceUID(const std::string& t_sopInstanceUid) { m_sopInstanceUid = t_sopInstanceUid; }
		void setClassUID(const std::string& t_classUid) { m_classUid = t_classUid; }
		void setFrameOfRefernceID(const std::string& t_frameOfReferenceId) { m_frameOfReferenceId = t_frameOfReferenceId; }
		void setModality(const std::string& t_modality) { m_modality = t_modality; }
		void setWindowCenter(const int& t_windowsCenter) { m_windowsCenter = t_windowsCenter; }
		void setWindowWidth(const int& t_windowWidth) { m_windowWidth = t_windowWidth; }
		void setRows(const int& t_rows) { m_rows = t_rows; }
		void setColumns(const int& t_columns) { m_columns = t_columns; }
		void setNumberOfFrames(const int& t_numberOfFrames) { m_numberOfFrames = t_numberOfFrames; }
		void setSliceLocation(const double& t_sliceLocation) { m_sliceLocation = t_sliceLocation; }
		void setIsMultiFrame(const bool& t_isMultiframe) { m_isMultiframe = t_isMultiframe; }

		bool operator==(const Image& t_rhs) const;

	private:
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
		bool m_isMultiframe = false;
	};
}
