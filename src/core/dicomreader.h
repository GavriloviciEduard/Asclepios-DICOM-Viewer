#pragma once

#include "imebra.h"
#include "patient.h"

namespace asclepios::core
{
	class DicomReader
	{
	public:
		DicomReader() = default;
		~DicomReader() = default;

		void readFile(const std::string& t_filePath);
		[[nodiscard]] std::unique_ptr<Patient> getReadPatient() const;
		[[nodiscard]] std::unique_ptr<Study> getReadStudy() const;
		[[nodiscard]] std::unique_ptr<Series> getReadSeries() const;
		[[nodiscard]] std::unique_ptr<Image> getReadImage() const;

	private:
		std::unique_ptr<imebra::DataSet> m_dataSet = {};
	};
}
