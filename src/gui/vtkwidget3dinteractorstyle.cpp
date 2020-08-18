#include "vtkwidget3dinteractorstyle.h"
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkwidget3d.h"

vtkStandardNewMacro(asclepios::gui::vtkWidget3DInteractorStyle);

void asclepios::gui::vtkWidget3DInteractorStyle::OnMouseMove()
{
	auto* const currentEventPosition =
		Interactor->GetEventPosition();
	auto* const lastEventPosition =
		Interactor->GetLastEventPosition();
	if (State == VTKIS_SPIN)
	{
		m_transferFunction->
			updateWindowLevel(currentEventPosition[0] - lastEventPosition[0],
				currentEventPosition[1] - lastEventPosition[1]);
		m_widget3D->updateFilter();
		Interactor->Render();
		return;
	}
	vtkInteractorStyleTrackballCamera::OnMouseMove();
}
