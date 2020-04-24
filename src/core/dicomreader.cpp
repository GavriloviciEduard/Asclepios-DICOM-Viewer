#include "dicomreader.h"
#include "utils.h"

//todo because dataset throws errors a method for reading must be implemented, if an error is thrown just return default data
void asclepios::core::DicomReader::readFile(const std::string& t_filePath)
{
	m_filePath = t_filePath;
	m_dataSet = std::make_unique<imebra::DataSet>(imebra::CodecFactory::load(t_filePath, 2048));
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Patient> asclepios::core::DicomReader::getReadPatient() const
{
	auto tempPatient = std::make_unique<Patient>();
	tempPatient->setAge(std::stoi(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientAge_0010_1010), 0)));
	tempPatient->setBirthDate(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientBirthDate_0010_0030), 0));
	tempPatient->setID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientID_0010_0020), 0));
	tempPatient->setName(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientName_0010_0010), 0));
	return tempPatient;
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Study> asclepios::core::DicomReader::getReadStudy() const
{
	auto tempStudy = std::make_unique<Study>();
	tempStudy->setID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::StudyID_0020_0010), 0));
	tempStudy->setDate(m_dataSet->getString(imebra::TagId(imebra::tagId_t::StudyDate_0008_0020), 0));
	tempStudy->setDescription(m_dataSet->getString(imebra::TagId(imebra::tagId_t::StudyDescription_0008_1030), 0));
	tempStudy->setUID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::StudyInstanceUID_0020_000D), 0));
	return tempStudy;
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Series> asclepios::core::DicomReader::getReadSeries() const
{
	auto tempSeries = std::make_unique<Series>();
	tempSeries->setUID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::SeriesInstanceUID_0020_000E), 0));
	tempSeries->setDate(m_dataSet->getString(imebra::TagId(imebra::tagId_t::SeriesDate_0008_0021), 0));
	tempSeries->setDescription(m_dataSet->getString(imebra::TagId(imebra::tagId_t::SeriesDescription_0008_103E), 0));
	tempSeries->setNumber(m_dataSet->getString(imebra::TagId(imebra::tagId_t::SeriesNumber_0020_0011), 0));
	return tempSeries;
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Image> asclepios::core::DicomReader::getReadImage()
{
	const auto modality = m_dataSet->getString(imebra::TagId(imebra::tagId_t::Modality_0008_0060), 0);
	if (!isModalitySupported(modality))
	{
		throw std::exception("Modality is not supported!");
	}
	auto tempImage = std::make_unique<Image>();
	tempImage->setModality(modality);
	tempImage->setImagePath(m_filePath);
	tempImage->setSOPInstanceUID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::SOPInstanceUID_0008_0018), 0));
	tempImage->setClassUID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::SOPClassUID_0008_0016), 0));
	const auto window = m_dataSet->getString(imebra::TagId(imebra::tagId_t::WindowCenter_0028_1050), 0);
	tempImage->setWindowCenter(window.empty() ? defaultWindow : std::stoi(window));
	const auto level = m_dataSet->getString(imebra::TagId(imebra::tagId_t::WindowWidth_0028_1051), 0);
	tempImage->setWindowWidth(level.empty() ? defaultLevel : std::stoi(level));
	const auto rows = m_dataSet->getString(imebra::TagId(imebra::tagId_t::Rows_0028_0010), 0);
	tempImage->setRows(rows.empty() ? 0 : std::stoi(rows));
	const auto columns = m_dataSet->getString(imebra::TagId(imebra::tagId_t::Columns_0028_0011), 0);
	tempImage->setColumns(columns.empty() ? 0 : std::stoi(columns));
	const auto sliceLocation = m_dataSet->getString(imebra::TagId(imebra::tagId_t::SliceLocation_0020_1041), 0);
	tempImage->setSliceLocation(sliceLocation.empty() ? 0 : std::stod(sliceLocation));
	const auto numberFrames = m_dataSet->getString(imebra::TagId(imebra::tagId_t::NumberOfFrames_0028_0008), 0);
	tempImage->setNumberOfFrames(numberFrames.empty() ? 0 : std::stoi(numberFrames));
	tempImage->setIsMultiFrame(tempImage->getNumberOfFrames() > 0);
	if (!tempImage->getIsMultiFrame())
	{
		tempImage->setFrameOfRefernceID(
			m_dataSet->getString(imebra::TagId(imebra::tagId_t::FrameOfReferenceUID_0020_0052), 0));
	}
	return tempImage;
}

//-----------------------------------------------------------------------------
bool asclepios::core::DicomReader::isModalitySupported(const std::string& t_modality)
{
	return t_modality != "PR" && t_modality != "KO" && t_modality != "SR";
}
