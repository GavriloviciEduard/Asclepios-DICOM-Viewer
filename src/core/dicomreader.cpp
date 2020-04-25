#include "dicomreader.h"
#include "utils.h"
#include <iostream>

void asclepios::core::DicomReader::readFile(const std::string& t_filePath)
{
	m_filePath = t_filePath;
	m_dataSet = std::make_unique<imebra::DataSet>(imebra::CodecFactory::load(t_filePath, 2048));
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Patient> asclepios::core::DicomReader::getReadPatient() const
{
	auto tempPatient = std::make_unique<Patient>();
	const auto age = getTagFromDataSet(imebra::tagId_t::PatientAge_0010_1010);
	tempPatient->setAge(age.empty() ? 0 : std::stoi(age));
	tempPatient->setBirthDate(getTagFromDataSet(imebra::tagId_t::PatientBirthDate_0010_0030));
	tempPatient->setID(getTagFromDataSet(imebra::tagId_t::PatientID_0010_0020));
	tempPatient->setName(getTagFromDataSet(imebra::tagId_t::PatientName_0010_0010));
	return tempPatient;
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Study> asclepios::core::DicomReader::getReadStudy() const
{
	auto tempStudy = std::make_unique<Study>();
	tempStudy->setID(getTagFromDataSet(imebra::tagId_t::StudyID_0020_0010));
	tempStudy->setDate(getTagFromDataSet(imebra::tagId_t::StudyDate_0008_0020));
	tempStudy->setDescription(getTagFromDataSet(imebra::tagId_t::StudyDescription_0008_1030));
	tempStudy->setUID(getTagFromDataSet(imebra::tagId_t::StudyInstanceUID_0020_000D));
	return tempStudy;
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Series> asclepios::core::DicomReader::getReadSeries() const
{
	auto tempSeries = std::make_unique<Series>();
	tempSeries->setUID(getTagFromDataSet(imebra::tagId_t::SeriesInstanceUID_0020_000E));
	tempSeries->setDate(getTagFromDataSet(imebra::tagId_t::SeriesDate_0008_0021));
	tempSeries->setDescription(getTagFromDataSet(imebra::tagId_t::SeriesDescription_0008_103E));
	tempSeries->setNumber(getTagFromDataSet(imebra::tagId_t::SeriesNumber_0020_0011));
	return tempSeries;
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Image> asclepios::core::DicomReader::getReadImage()
{
	const auto modality = getTagFromDataSet(imebra::tagId_t::Modality_0008_0060);
	if (!isModalitySupported(modality))
	{
		throw std::exception("Modality is not supported!");
	}
	auto tempImage = std::make_unique<Image>();
	tempImage->setModality(modality);
	tempImage->setImagePath(m_filePath);
	tempImage->setSOPInstanceUID(getTagFromDataSet(imebra::tagId_t::SOPInstanceUID_0008_0018));
	tempImage->setClassUID(getTagFromDataSet(imebra::tagId_t::SOPClassUID_0008_0016));
	const auto window = getTagFromDataSet(imebra::tagId_t::WindowCenter_0028_1050);
	tempImage->setWindowCenter(window.empty() ? defaultWindow : std::stoi(window));
	const auto level = getTagFromDataSet(imebra::tagId_t::WindowWidth_0028_1051);
	tempImage->setWindowWidth(level.empty() ? defaultLevel : std::stoi(level));
	const auto rows = getTagFromDataSet(imebra::tagId_t::Rows_0028_0010);
	tempImage->setRows(rows.empty() ? 0 : std::stoi(rows));
	const auto columns = getTagFromDataSet(imebra::tagId_t::Columns_0028_0011);
	tempImage->setColumns(columns.empty() ? 0 : std::stoi(columns));
	const auto sliceLocation = getTagFromDataSet(imebra::tagId_t::SliceLocation_0020_1041);
	tempImage->setSliceLocation(sliceLocation.empty() ? 0 : std::stod(sliceLocation));
	const auto numberFrames = getTagFromDataSet(imebra::tagId_t::NumberOfFrames_0028_0008);
	tempImage->setNumberOfFrames(numberFrames.empty() ? 0 : std::stoi(numberFrames));
	tempImage->setIsMultiFrame(tempImage->getNumberOfFrames() > 0);
	if (!tempImage->getIsMultiFrame())
	{
		tempImage->setFrameOfRefernceID(getTagFromDataSet(imebra::tagId_t::FrameOfReferenceUID_0020_0052));
	}
	return tempImage;
}

//-----------------------------------------------------------------------------
std::string asclepios::core::DicomReader::getTagFromDataSet(const imebra::tagId_t& t_tagId) const
{
	try
	{
		return m_dataSet->getString(imebra::TagId(t_tagId), 0);
	}
	catch (std::exception& ex)
	{
		//todo log exception
		std::cout << ex.what() << '\n';
		return "";
	}
}

//-----------------------------------------------------------------------------
bool asclepios::core::DicomReader::isModalitySupported(const std::string& t_modality)
{
	return t_modality != "PR" && t_modality != "KO" && t_modality != "SR";
}
