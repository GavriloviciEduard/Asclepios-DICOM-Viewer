#include "widgetscontroller.h"
#include "filesimporter.h"
#include "widget2d.h"

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
	resetConnections();
	createRemoveWidgets(computeNumberWidgetsFromLayout(t_layout));
	m_widgetsContainer->setLayout(t_layout);
	createConnections();
	(*m_widgetsRepository->getWidgets().begin())->onFocus(true);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::applyTransformation(const transformationType& t_type) const
{
	if (auto* const widget2d = dynamic_cast<Widget2D*>(m_activeWidget->getTabbedWidget());
		widget2d && widget2d->getWidgetType() == WidgetBase::WidgetType::widget2d
		&& widget2d->getFuture().isFinished() && widget2d->getImage())
	{
		dynamic_cast<vtkWidget2D*>(widget2d->getWidgetVTK())->applyTransformation(t_type);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::resetData() const
{
	waitForRenderingThreads();
	const auto widgets = m_widgetsRepository->getWidgets();
	for (const auto& widget : widgets)
	{
		if (widget->getTabbedWidget()->getImage())
		{
			widget->resetWidget();
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::waitForRenderingThreads() const
{
	const auto widgets = m_widgetsRepository->getWidgets();
	for (const auto& widget : widgets)
	{
		dynamic_cast<Widget2D*>(widget->getTabbedWidget())
			->getFuture().waitForFinished();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::setActiveWidget(TabWidget* t_widget)
{
	if (m_activeWidget)
	{
		m_activeWidget->onFocus(false);
	}
	m_activeWidget = t_widget;
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::setMaximize(TabWidget* t_widget) const
{
	const auto widgets = m_widgetsRepository->getWidgets();
	t_widget->setIsMaximized(!t_widget->getIsMaximized());
	for (const auto& widget : widgets)
	{
		if (t_widget != widget)
		{
			widget->setVisible(!t_widget->getIsMaximized());
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::populateWidget(core::Series* t_series, core::Image* t_image) const
{
	auto* const widget = findNextAvailableWidget();
	if (widget)
	{
		auto* const widget2d = dynamic_cast<Widget2D*>(widget->getActiveTabbedWidget());
		widget2d->setWidgetType(WidgetBase::WidgetType::widget2d);
		widget2d->setSeries(t_series);
		widget2d->setImage(t_image);
		auto* const study = t_series->getParentObject();
		widget2d->setIndexes(study->getParentObject()->getIndex(),
			study->getIndex(), t_series->getIndex(),
			t_image->getIndex());
		widget2d->setIsImageLoaded(true);
		widget2d->render();
		Q_UNUSED(connect(m_filesImporter,&FilesImporter::refreshScrollValues,
			widget2d,&Widget2D::refreshScrollValues));
	}
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
void asclepios::gui::WidgetsController::createConnections() const
{
	const auto widgets = m_widgetsRepository->getWidgets();
	for (const auto& widget : widgets)
	{
		Q_UNUSED(connect(widget, &TabWidget::focused, this,
			&WidgetsController::setActiveWidget));
		Q_UNUSED(connect(widget, &TabWidget::setMaximized, this,
			&WidgetsController::setMaximize));
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsController::resetConnections()
{
	m_activeWidget = nullptr;
	const auto widgets = m_widgetsRepository->getWidgets();
	for (const auto& widget : widgets)
	{
		widget->onFocus(false);
		widget->setIsMaximized(false);
		widget->setVisible(true);
		disconnect(widget, &TabWidget::focused, this,
		           &WidgetsController::setActiveWidget);
		disconnect(widget, &TabWidget::setMaximized, this,
		           &WidgetsController::setMaximize);
		if (auto* const widget2d = dynamic_cast<Widget2D*>(widget->getTabbedWidget()); widget2d)
		{
			disconnect(m_filesImporter, &FilesImporter::refreshScrollValues,
			           widget2d, &Widget2D::refreshScrollValues);
		}
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
asclepios::gui::TabWidget* asclepios::gui::WidgetsController::findNextAvailableWidget() const
{
	const auto widgets = m_widgetsRepository->getWidgets();
	const auto it = std::find_if(widgets.begin(),
	                             widgets.end(), [](TabWidget* t_widget)
	                             {
		                             return !t_widget->getActiveTabbedWidget()->getIsImageLoaded();
	                             });
	return it == widgets.end() ? nullptr : *it;
}

//-----------------------------------------------------------------------------
std::size_t asclepios::gui::WidgetsController::computeNumberWidgetsFromLayout(const WidgetsContainer::layouts& t_layout)
{
	return t_layout == WidgetsContainer::layouts::one ? 1 : static_cast<std::size_t>(t_layout) / 2 + 2ul;
}
