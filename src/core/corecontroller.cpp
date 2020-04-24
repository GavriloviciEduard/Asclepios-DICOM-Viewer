#include "corecontroller.h"
#include <iostream>

asclepios::core::CoreController::CoreController()
{
	InitData();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::readData(const std::string& t_filepath) const
{
	try
	{
		m_dicomReader->readFile(t_filepath);
		if (m_dicomReader->dataSetExists())
		{
			InsertData();
		}
		else
		{
			throw std::exception("File not supported");
		}
	}
	catch (std::exception& ex)
	{
		//todo log exception
		std::cout << ex.what() << '\n';
	}
}

//-----------------------------------------------------------------------------
std::set<std::unique_ptr<asclepios::core::Patient>, asclepios::core::Patient::patientCompare>& asclepios::core::
CoreController::getPatients() const
{
	return m_coreRepository->getPatients();
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
