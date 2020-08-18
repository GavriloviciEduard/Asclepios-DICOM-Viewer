#include "vtkreslicerotator.h"
#include <vtkNew.h>
#include <vtkTransform.h>

void asclepios::gui::vtkResliceRotator::rotatePlane(vtkMatrix4x4* t_sourceMatrix,
                                                    vtkMatrix4x4* t_destinationMatrix, const double t_angle)
{
	vtkNew<vtkTransform> transform2;
	vtkNew<vtkTransform> destinationPlaneTransform;
	destinationPlaneTransform->SetMatrix(t_destinationMatrix);
	double defaultMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	double* destinationPlaneOrientation = destinationPlaneTransform->GetOrientation();
	const double xRotation = destinationPlaneOrientation[0];
	const double yRotation = destinationPlaneOrientation[1];
	const double zRotation = destinationPlaneOrientation[2];
	const double xCenter = t_destinationMatrix->GetElement(0, 3);
	const double yCenter = t_destinationMatrix->GetElement(1, 3);
	const double zCenter = t_destinationMatrix->GetElement(2, 3);
	transform2->SetMatrix(defaultMatrix);
	transform2->Translate(xCenter, yCenter, zCenter);
	transform2->RotateWXYZ(t_angle,
	                       t_sourceMatrix->GetElement(0, 2),
	                       t_sourceMatrix->GetElement(1, 2),
	                       t_sourceMatrix->GetElement(2, 2));
	transform2->RotateZ(zRotation);
	transform2->RotateX(xRotation);
	transform2->RotateY(yRotation);
	t_destinationMatrix->DeepCopy(transform2->GetMatrix());
}
