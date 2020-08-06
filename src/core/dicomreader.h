#pragma once

#include "patient.h"
#include <dcmtk/dcmdata/dcfilefo.h>

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
		[[nodiscard]] bool dataSetExists() const { return m_dataSet != nullptr; }

	private:
		std::unique_ptr<DcmFileFormat> m_file = {};
		DcmDataset* m_dataSet = {};
		std::string m_filePath = {};

		[[nodiscard]] std::string getTagFromDataSet(const DcmTagKey& tagKey) const;
		[[nodiscard]] static bool isModalitySupported(const std::string& t_modality);
		[[nodiscard]] std::tuple<int, int> getWindowLevel() const;
		[[nodiscard]] std::tuple<double, double> getPixelSpacing() const;
	};
}
