#include "vtkwidgetbase.h"

void asclepios::gui::vtkWidgetBase::setRenderWindowsMPR(const vtkSmartPointer<vtkRenderWindow>& t_sagittal,
	const vtkSmartPointer<vtkRenderWindow>& t_coronal,
	const vtkSmartPointer<vtkRenderWindow>& t_axial)
{
	m_renderWindows[0] = t_sagittal;
	m_renderWindows[1] = t_coronal;
	m_renderWindows[2] = t_axial;
}
