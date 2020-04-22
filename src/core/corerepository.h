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

	private:
		std::unordered_set<std::unique_ptr<Patient>> m_patients = {};
	};
}
