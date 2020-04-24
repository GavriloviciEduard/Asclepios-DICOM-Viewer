#pragma once

#include <QAbstractNativeEventFilter>

class QWindow;


namespace asclepios::gui
{
	class NativeWindowHelper;
	class NativeWindowFilter : public QAbstractNativeEventFilter
	{
	public:
		static void deliver(QWindow* window, NativeWindowHelper* helper);
	protected:
		bool nativeEventFilter(const QByteArray& eventType,
		                       void* message, long* result) final;
	};
}
