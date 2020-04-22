#include "dicomreader.h"

void asclepios::core::DicomReader::readFile(const std::string& t_filePath)
{
	m_dataSet = std::make_unique<imebra::DataSet>(imebra::CodecFactory::load(t_filePath, 2048));
}

//-----------------------------------------------------------------------------
std::unique_ptr<asclepios::core::Patient> asclepios::core::DicomReader::getPatient() const
{
	auto tempPatient = std::make_unique<Patient>();
	tempPatient->setAge(m_dataSet->getInt32(imebra::TagId(imebra::tagId_t::PatientAge_0010_1010), 0));
	tempPatient->setBirthDate(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientBirthDate_0010_0030), 0));
	tempPatient->setID(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientID_0010_0020), 0));
	tempPatient->setName(m_dataSet->getString(imebra::TagId(imebra::tagId_t::PatientName_0010_0010), 0));
	return std::move(tempPatient);
}
