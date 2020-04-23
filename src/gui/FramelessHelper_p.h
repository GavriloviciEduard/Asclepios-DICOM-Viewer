#pragma once

#include <QSet>
#include "FramelessHelper.h"
#include "NativeWindowHelper.h"

namespace asclepios::gui
{
	class FramelessHelperPrivate : public NativeWindowTester
	{
	public:
		FramelessHelperPrivate();
		virtual ~FramelessHelperPrivate();

		[[nodiscard]] QMargins draggableMargins() const final;
		[[nodiscard]] QMargins maximizedMargins() const final;
		[[nodiscard]] bool hitTest(const QPoint& pos) const final;

		QWidget* window;
		NativeWindowHelper* helper;
		QMargins priDraggableMargins;
		QMargins priMaximizedMargins;
		QSet<QWidget*> extraTitleBars;
		int titleBarHeight;
		QSet<QWidget*> includeItems;
		QSet<QWidget*> excludeItems;
		bool maximized;
	};
}
