#pragma once

#include <memory>
#include "patient.h"

namespace asclepios::core
{
	class CoreRepository
	{
	public:
		CoreRepository() = default;
		~CoreRepository() = default;

		//getters
		[[nodiscard]] std::set<std::unique_ptr<Patient>, Patient::patientCompare>& getPatients() { return m_patients; }

		//add
		void addPatient(std::unique_ptr<Patient> t_patient);
		void addStudy(std::unique_ptr<Study> t_study);
		void addSeries(std::unique_ptr<Series> t_series);
		void addImage(std::unique_ptr<Image> t_image) const;

		//delete
		void deletePatient(Patient* t_patient);
		void deleteAllPatients();

	private:
		std::set<std::unique_ptr<Patient>, Patient::patientCompare> m_patients = {};
		Patient* m_lastPatient = {};
		Study* m_lastStudy = {};
		Series* m_lastSeries = {};
	};
}
