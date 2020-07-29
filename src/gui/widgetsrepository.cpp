#include "widgetsrepository.h"

void asclepios::gui::WidgetsRepository::removeWidget()
{
	auto* widget = m_widgets.back();
	delete widget;
	widget = nullptr;
	m_widgets.pop_back();
}
