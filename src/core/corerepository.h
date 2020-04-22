#pragma once

#include <unordered_set>
#include <memory>
#include "patient.h"

namespace asclepios::core
{
	class CoreRepository
	{
	public:
		CoreRepository() = default;
		~CoreRepository() = default;

		void addPatient(std::unique_ptr<Patient> t_patient) { m_patients.emplace(std::move(t_patient)); }
		void addStudy(std::unique_ptr<Study> t_study);
		void addSeries(std::unique_ptr<Series> t_series);
		void addImage(std::unique_ptr<Image> t_image);

	private:
		std::unordered_set<std::unique_ptr<Patient>> m_patients = {};
	};
}
