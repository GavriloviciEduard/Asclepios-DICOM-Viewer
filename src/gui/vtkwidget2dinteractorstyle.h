#pragma once

#include <vtkInteractorStyleImage.h>

namespace asclepios::gui
{
	class vtkWidget2DInteractorStyle : public vtkInteractorStyleImage
	{
	public:
		static vtkWidget2DInteractorStyle* New();
		vtkTypeMacro(vtkWidget2DInteractorStyle, vtkInteractorStyleImage);

		vtkWidget2DInteractorStyle() = default;
		~vtkWidget2DInteractorStyle() = default;
	private:
	};
}
