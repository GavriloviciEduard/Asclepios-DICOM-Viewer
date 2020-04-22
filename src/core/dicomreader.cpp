#include "dicomreader.h"

void asclepios::core::DicomReader::readFile(const std::string& t_filePath)
{
	m_dataSet = std::make_unique<imebra::DataSet>(imebra::CodecFactory::load(t_filePath, 2048));
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Patient> asclepios::core::DicomReader::getReadPatient() const
{
	auto tempPatient = std::make_unique<Patient>();
	tempPatient->setAge(m_dataSet->getInt32(imebra::TagId(imebra::tagId_t::PatientAge_0010_1010), 0));
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
std::unique_ptr<asclepios::core::Image> asclepios::core::DicomReader::getReadImage() const
{
	//todo read other req data 
	auto tempImage = std::make_unique<Image>();
	//tempImage->setImage(m_dataSet->getImage(0));
	return tempImage;
}
