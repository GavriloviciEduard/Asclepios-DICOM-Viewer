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

	private:
		std::unique_ptr<CoreRepository> m_coreRepository = {};
		std::unique_ptr<DicomReader> m_dicomReader = {};

		void InitData();
		void InsertData() const;
	};
}
