#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include "transferfunction.h"

namespace asclepios::gui
{
	class vtkWidget3D;

	class vtkWidget3DInteractorStyle final : public vtkInteractorStyleTrackballCamera
	{
	public:
		static vtkWidget3DInteractorStyle* New();
		vtkTypeMacro(vtkWidget3DInteractorStyle, vtkInteractorStyleTrackballCamera);
		vtkWidget3DInteractorStyle() = default;
		~vtkWidget3DInteractorStyle() = default;

		//getters
		[[nodiscard]] vtkWidget3D* getWidget() const { return  m_widget3D; }
		[[nodiscard]] TransferFunction* getTransferFunction() const { return m_transferFunction; }

		//setters
		void setWidget(vtkWidget3D* t_widget) { m_widget3D = t_widget; }
		void setTransferFunction(TransferFunction* t_function) { m_transferFunction = t_function; }

	protected:
		void OnMouseMove() override;

	private:
		vtkWidget3D* m_widget3D = {};
		TransferFunction* m_transferFunction = {};
	};
}
