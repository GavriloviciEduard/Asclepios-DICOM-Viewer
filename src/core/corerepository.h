#pragma once

#include <memory>
#include "patient.h"
#include <vector>

namespace asclepios::core
{
	class CoreRepository
	{
	public:
		CoreRepository() = default;
		~CoreRepository() = default;

		//getters
		[[nodiscard]] std::vector<std::unique_ptr<Patient>>& getPatients() { return m_patients; }
		[[nodiscard]] Patient* getLastPatient() const { return m_lastPatient; }
		[[nodiscard]] Study* getLastStudy() const { return m_lastStudy; }
		[[nodiscard]] Series* getLastSeries() const { return m_lastSeries; }
		[[nodiscard]] Image* getLastImage()  const { return m_lastImage; }
		[[nodiscard]] bool newSeriesAdded() const { return m_newSeries; }
		[[nodiscard]] bool newImageAdded() const { return m_newImage; }

		//add
		void addPatient(std::unique_ptr<Patient> t_patient);
		void addStudy(std::unique_ptr<Study> t_study);
		void addSeries(std::unique_ptr<Series> t_series);
		void addImage(std::unique_ptr<Image> t_image);

		//delete
		void deletePatient(Patient* t_patient);
		void deleteAllPatients() { m_patients.clear(); }
		void resetLastPatientData();

		//find
		[[nodiscard]] std::size_t findPatient(Patient* t_patient);

	private:
		std::vector<std::unique_ptr<Patient>> m_patients = {};
		Patient* m_lastPatient = {};
		Study* m_lastStudy = {};
		Series* m_lastSeries = {};
		Image* m_lastImage = {};
		bool m_newSeries = false;
		bool m_newImage = false;
	};
}
