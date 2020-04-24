#pragma once

#include "NativeWindowHelper.h"

namespace asclepios::gui
{
	class NativeWindowHelperPrivate
	{
		Q_DECLARE_PUBLIC(NativeWindowHelper)

	public:
		NativeWindowHelperPrivate();
		virtual ~NativeWindowHelperPrivate();
	protected:
		NativeWindowHelper* q_ptr;

	public:
		void updateWindowStyle();
		int hitTest(int x, int y) const;
		bool isMaximized() const;

		QMargins draggableMargins() const;
		QMargins maximizedMargins() const;
		QWindow* window;
		NativeWindowTester* tester;
		HWND oldWindow;
		QRect availableGeometry() const;
		qreal scaleFactor;
	};
}
