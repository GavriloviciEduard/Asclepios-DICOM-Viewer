#include "vtkreslicetranslator.h"
#include <vtkNew.h>
#include <vtkTransform.h>
#include "vtkMatrix4x4.h"

void asclepios::gui::vtkResliceTranslator::movePlaneX(vtkMatrix4x4* t_sourceMatrix,
                                                      vtkMatrix4x4* t_destinationMatrix1,
                                                      vtkMatrix4x4* t_destinationMatrix2, double* t_point)
{
	vtkNew<vtkTransform> transform;
	transform->SetMatrix(t_sourceMatrix);
	transform->Translate(t_point[0], t_point[1], 0);
	double center[3] =
	{
		transform->GetMatrix()->GetElement(0, 3),
		transform->GetMatrix()->GetElement(1, 3),
		transform->GetMatrix()->GetElement(2, 3)
	};
	t_destinationMatrix1->SetElement(0, 3, center[0]);
	t_destinationMatrix1->SetElement(1, 3, center[1]);
	t_destinationMatrix1->SetElement(2, 3, center[2]);
	t_destinationMatrix2->SetElement(0, 3, center[0]);
	t_destinationMatrix2->SetElement(1, 3, center[1]);
	t_destinationMatrix2->SetElement(2, 3, center[2]);
}
