#pragma once

#include <memory>
#include "corerepository.h"
#include "utils.h"
#include "dicomreader.h"

namespace asclepios::core
{
	class export CoreController
	{
	public:
		CoreController();
		~CoreController() = default;

		void readData(const std::string& t_filepath) const;
		[[nodiscard]] std::vector<std::unique_ptr<Patient>>& getPatients() const { return m_coreRepository->getPatients(); }
		[[nodiscard]] Patient* getLastPatient() const { return m_coreRepository->getLastPatient(); }
		[[nodiscard]] int getLastPatientIndex() const { return m_coreRepository->getLastPatient()->getIndex(); }
		[[nodiscard]] Study* getLastStudy() const { return m_coreRepository->getLastStudy(); }
		[[nodiscard]] int getLastStudyIndex() const { return m_coreRepository->getLastStudy()->getIndex(); }
		[[nodiscard]] Series* getLastSeries() const { return m_coreRepository->getLastSeries(); }
		[[nodiscard]] int getLastSeriesSize() const;
		[[nodiscard]] int getLastSeriesIndex() const { return m_coreRepository->getLastSeries()->getIndex(); }
		[[nodiscard]] Image* getLastImage()  const { return m_coreRepository->getLastImage(); }
		[[nodiscard]] int getLastImageIndex() const { return m_coreRepository->getLastImage()->getIndex(); }
		[[nodiscard]] bool newSeriesAdded() const { return m_coreRepository->newSeriesAdded(); }
		[[nodiscard]] bool newImageAdded() const { return m_coreRepository->newImageAdded(); }

	private:
		std::unique_ptr<CoreRepository> m_coreRepository = {};
		std::unique_ptr<DicomReader> m_dicomReader = {};

		void initData();
		void insertDataInRepo() const;
	};
}
