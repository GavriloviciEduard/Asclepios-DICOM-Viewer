#pragma once

#include <vtkObjectFactory.h>
#include <vtkCommand.h>

namespace asclepios::gui
{
	class vtkWidgetOverlay;

	class vtkWidgetOverlayCallback final : public vtkCommand
	{
	public:
		static vtkWidgetOverlayCallback* New();
		vtkTypeMacro(vtkWidgetOverlayCallback, vtkCommand);

		vtkWidgetOverlayCallback() = default;
		~vtkWidgetOverlayCallback() = default;

		//setters
		void setWidget(vtkWidgetOverlay* t_widget) { m_widgetOverlay = t_widget; }

	protected:
		void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;

	private:
		vtkWidgetOverlay* m_widgetOverlay = {};
	};
}
