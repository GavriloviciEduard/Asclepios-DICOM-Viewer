#pragma once

#include "tabwidget.h"

namespace asclepios::gui
{
	class WidgetsRepository
	{
	public:
		WidgetsRepository() = default;
		~WidgetsRepository() = default;

		//getters
		[[nodiscard]] std::vector<TabWidget*>& getWidgets() { return m_widgets; }

		void addWidget(TabWidget* t_widget) { m_widgets.push_back(t_widget); }
		void removeWidget();

	private:
		std::vector<TabWidget*> m_widgets = {};
	};
}