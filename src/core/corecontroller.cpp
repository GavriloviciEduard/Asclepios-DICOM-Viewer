#include "corecontroller.h"

asclepios::core::CoreController::CoreController()
{
	InitData();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::readData(const std::string& t_filepath) const
{
	m_dicomReader->readFile(t_filepath);
	InsertData();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::InitData()
{
	m_coreRepository = std::make_unique<CoreRepository>();
	m_dicomReader = std::make_unique<DicomReader>();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::InsertData() const
{
	m_coreRepository->addPatient(m_dicomReader->getReadPatient());
	m_coreRepository->addStudy(m_dicomReader->getReadStudy());
	m_coreRepository->addSeries(m_dicomReader->getReadSeries());
	m_coreRepository->addImage(m_dicomReader->getReadImage());
}
