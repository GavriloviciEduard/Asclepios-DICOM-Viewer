#include "vtkeventfilter.h"
#include <QEvent>

bool asclepios::gui::vtkEventFilter::eventFilter(QObject* t_watched, QEvent* t_event)
{
	if (t_event->type() == t_event->MouseButtonPress ||
		t_event->type() == t_event->Wheel)
	{
		emit activateWidget(true, t_watched);
	}
	return QObject::eventFilter(t_watched, t_event);
}
