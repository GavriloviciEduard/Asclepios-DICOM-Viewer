#pragma once

#include <QSet>
#include "WindowFramelessHelper.h"
#include "NativeWindowHelper.h"

namespace asclepios::gui
{
	class WindowFramelessHelperPrivate : public NativeWindowTester
	{
	public:
		WindowFramelessHelperPrivate();
		virtual ~WindowFramelessHelperPrivate();

		[[nodiscard]] QMargins draggableMargins() const final;
		[[nodiscard]] QMargins maximizedMargins() const final;
		[[nodiscard]] bool hitTest(const QPoint& pos) const final;

		QQuickWindow* window;
		NativeWindowHelper* helper;
		QMargins priDraggableMargins;
		QMargins priMaximizedMargins;
		QSet<QQuickItem*> extraTitleBars;
		int titleBarHeight;
		QSet<QQuickItem*> includeItems;
		QSet<QQuickItem*> excludeItems;
	};
}
