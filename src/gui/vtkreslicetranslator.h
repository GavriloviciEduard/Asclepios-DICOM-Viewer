#pragma once

class vtkImageResliceToColors;
class vtkImageResliceToColor;
class vtkMatrix4x4;

namespace asclepios::gui
{
	class vtkResliceTranslator
	{
	public:
		vtkResliceTranslator() = default;
		~vtkResliceTranslator() = default;
		static void movePlaneX(vtkMatrix4x4*,
			vtkMatrix4x4*, vtkMatrix4x4*, double* t_point);
	};
}
		

