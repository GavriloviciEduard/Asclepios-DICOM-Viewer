#pragma once

#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

namespace asclepios::gui
{
	class vtkWidgetMPR;

	class vtkWidgetMPRCallback final : public vtkCommand
	{
	public:
		static vtkWidgetMPRCallback* New();
		vtkTypeMacro(vtkWidgetMPRCallback, vtkCommand);
		vtkWidgetMPRCallback() = default;
		~vtkWidgetMPRCallback() = default;

		//setters
		void setWidget(vtkWidgetMPR* t_widget) { m_widget = t_widget; }

		void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;
		
	private:
		vtkWidgetMPR* m_widget = {};
	};
}
