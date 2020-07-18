#include "widgetsrepository.h"

void asclepios::gui::WidgetsRepository::removeWidget()
{
	auto* const widget = m_widgets.back();
	delete widget;
	m_widgets.pop_back();
}
