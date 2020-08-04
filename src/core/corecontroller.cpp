#include "corecontroller.h"

asclepios::core::CoreController::CoreController()
{
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::readData(const std::string& t_filepath) const
{
	try
	{
		m_dicomReader->readFile(t_filepath);
		m_dicomReader->dataSetExists()
			? insertDataInRepo()
			: throw std::exception("File not supported");
	}
	catch (std::exception& ex)
	{
		//todo log exception
	}
}

//-----------------------------------------------------------------------------
int asclepios::core::CoreController::getLastSeriesSize() const
{
	auto* const image = m_coreRepository->getLastImage();
	return image->getIsMultiFrame()
		? image->getNumberOfFrames()
		: static_cast<int>(m_coreRepository->
			getLastSeries()->getSinlgeFrameImages().size());
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::resetData()
{
	m_coreRepository.reset();
	m_coreRepository = nullptr;
	m_coreRepository = std::make_unique<CoreRepository>();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::initData()
{
	m_coreRepository = std::make_unique<CoreRepository>();
	m_dicomReader = std::make_unique<DicomReader>();
}

//-----------------------------------------------------------------------------
void asclepios::core::CoreController::insertDataInRepo() const
{
	m_coreRepository->addPatient(m_dicomReader->getReadPatient());
	m_coreRepository->addStudy(m_dicomReader->getReadStudy());
	m_coreRepository->addSeries(m_dicomReader->getReadSeries());
	if(m_coreRepository->getLastSeries())
	{
		m_coreRepository->addImage(m_dicomReader->getReadImage());
	}
}
