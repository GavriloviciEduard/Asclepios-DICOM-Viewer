#include "NativeWindowFilter.h"
#include "NativeWindowFilter_p.h"

#include <windows.h>
#include <QCoreApplication>

#include "NativeWindowHelper_p.h"

// class NativeWindowFilter

void asclepios::gui::NativeWindowFilter::deliver(QWindow* window, NativeWindowHelper* helper)
{
	Q_CHECK_PTR(window);

	if (!NativeWindowFilterPrivate::instance)
	{
		QCoreApplication* appc = QCoreApplication::instance();
		if (appc)
		{
			const auto filter = new NativeWindowFilter();
			NativeWindowFilterPrivate::instance.reset(filter);
			appc->installNativeEventFilter(filter);
		}
	}

	if (helper)
	{
		const WId newId = window->winId();
		const WId oldId = NativeWindowFilterPrivate::windows.value(helper);
		if (newId != oldId)
		{
			NativeWindowFilterPrivate::helpers.insert(newId, helper);
			NativeWindowFilterPrivate::helpers.remove(oldId);
			NativeWindowFilterPrivate::windows.insert(helper, newId);

			NativeWindowFilterPrivate::winIds.insert(window, newId);
		}
	}
	else
	{
		const WId oldId = NativeWindowFilterPrivate::winIds.take(window);
		NativeWindowHelper* window_helper = NativeWindowFilterPrivate::helpers.take(oldId);
		NativeWindowFilterPrivate::windows.remove(window_helper);
	}
}

bool asclepios::gui::NativeWindowFilter::nativeEventFilter(const QByteArray& eventType,
                                                           void* message, long* result)
{
	Q_UNUSED(eventType);

	auto msg = reinterpret_cast<LPMSG>(message);
	if (auto h = NativeWindowFilterPrivate::helpers.value(reinterpret_cast<WId>(msg->hwnd)))
		return h->nativeEventFilter(msg, result);

	return false;
}

// class NativeWindowFilterPrivate

QScopedPointer<asclepios::gui::NativeWindowFilter> asclepios::gui::NativeWindowFilterPrivate::instance;

QHash<asclepios::gui::NativeWindowHelper*, WId> asclepios::gui::NativeWindowFilterPrivate::windows;
QHash<QWindow*, WId> asclepios::gui::NativeWindowFilterPrivate::winIds;
QHash<WId, asclepios::gui::NativeWindowHelper*> asclepios::gui::NativeWindowFilterPrivate::helpers;
