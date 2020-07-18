#include "widgetscontroller.h"

asclepios::gui::WidgetsController::WidgetsController()
{
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::initData()
{
	m_widgetsRepository = std::make_unique<WidgetsRepository>();
	m_widgetsContainer = std::make_unique<WidgetsContainer>();
	m_widgetsContainer->setWidgetReference(&m_widgetsRepository->getWidgets());
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::createWidgets(const WidgetsContainer::layouts& t_layout,
                                                      const int& t_nrWidgets) const
{
	createRemoveWidgets(t_nrWidgets);
	m_widgetsContainer->setLayout(t_layout);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::createRemoveWidgets(const int& t_nrWidgets) const
{
	while (t_nrWidgets != m_widgetsRepository->getWidgets().size())
	{
		m_widgetsRepository->getWidgets().size() > t_nrWidgets
			? m_widgetsRepository->removeWidget()
			: m_widgetsRepository->addWidget(createNewWidget());
	}
}

//-----------------------------------------------------------------------------
asclepios::gui::TabWidget* asclepios::gui::WidgetsController::createNewWidget() const
{
	auto* const widget = new TabWidget(m_widgetsContainer.get());
	widget->createWidget2D();
	return widget;
}
