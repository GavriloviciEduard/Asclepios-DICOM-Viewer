#pragma once

#include <QTabWidget>
#include "ui_patienttab.h"

namespace asclepios::gui
{
	class PatientTab : public QTabWidget
	{
	Q_OBJECT

	public:
		explicit PatientTab(QWidget* parent = Q_NULLPTR);
		~PatientTab() = default;

	private:
		Ui::PatientTab m_ui = {};
	};
}
