#include "vtkeventfilter.h"
#include <QVariant>
#include <QEvent>

bool asclepios::gui::vtkEventFilter::eventFilter(QObject* t_watched, QEvent* t_event)
{
	if (t_event->type() == t_event->MouseButtonPress ||
		t_event->type() == t_event->Wheel)
	{
		t_watched->setProperty("activate", true);
		emit activateWidget(true, t_watched);
	}
	if (t_event->type() == t_event->MouseButtonDblClick)
	{
		emit setMaximized();
	}
	return QObject::eventFilter(t_watched, t_event);
}
