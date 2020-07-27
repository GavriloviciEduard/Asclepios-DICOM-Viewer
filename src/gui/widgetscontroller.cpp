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
void asclepios::gui::WidgetsController::createWidgets(const WidgetsContainer::layouts& t_layout)
{
	createRemoveWidgets(computeNumberWidgetsFromLayout(t_layout));
	m_widgetsContainer->setLayout(t_layout);
	auto* const firstWidget = *m_widgetsRepository->getWidgets().begin();
	firstWidget->getActiveTabbedWidget()->setFocus();
	m_activeWidget = firstWidget;
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::createRemoveWidgets(const std::size_t& t_nrWidgets) const
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

//-----------------------------------------------------------------------------
std::size_t asclepios::gui::WidgetsController::computeNumberWidgetsFromLayout(const WidgetsContainer::layouts& t_layout)
{
	return t_layout == WidgetsContainer::layouts::one ? 1 : static_cast<std::size_t>(t_layout) / 2 + 2ul;
}
