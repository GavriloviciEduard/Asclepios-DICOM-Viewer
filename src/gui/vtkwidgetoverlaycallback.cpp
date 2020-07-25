#include "vtkwidgetoverlaycallback.h"
#include "vtkwidgetoverlay.h"

vtkStandardNewMacro(asclepios::gui::vtkWidgetOverlayCallback);

void asclepios::gui::vtkWidgetOverlayCallback::Execute([[maybe_unused]] vtkObject* caller, const unsigned long eventId,
                                                       [[maybe_unused]] void* callData)
{
	if (eventId == ModifiedEvent)
	{
		m_widgetOverlay->positionOverlay();
	}
}
