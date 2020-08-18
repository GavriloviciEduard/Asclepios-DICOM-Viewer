#pragma once

#include <vtkCommand.h>
#include "vtkreslicewidget.h"

namespace asclepios::gui
{
	class vtkResliceCallback final : public vtkCommand
	{
	public:
		static vtkResliceCallback* New();
		vtkTypeMacro(vtkResliceCallback, vtkCommand);
		vtkResliceCallback() = default;
		~vtkResliceCallback() = default;


		//getters
		[[nodiscard]] vtkResliceWidget* getWidget() const { return m_resliceWidget; }
		[[nodiscard]] vtkRenderWindow* getRenderWindow() const { return m_window; }
		[[nodiscard]] int getHandleNumber() const { return m_handleNumber; }

		//setters
		void setWidget(vtkResliceWidget* t_widget) { m_resliceWidget = t_widget; }
		void setRenderWindow(vtkRenderWindow* t_window) { m_window = t_window; }
		void setHandleNumber(const int& t_nr) { m_handleNumber = t_nr; }

		void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;

	private:
		vtkResliceWidget* m_resliceWidget = {};
		vtkRenderWindow* m_window = {};
		int m_handleNumber = {};

		void setCursorPositiontoDefault() const;
		void rotateCursor(double t_angle) const;
		void moveCursor(double* t_position) const;
		void changeMatrixCenterPosition(
			vtkMatrix4x4* t_sourceMatrix, vtkMatrix4x4* t_destinationMatrix);
	};
}
