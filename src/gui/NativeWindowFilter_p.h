#pragma once

#include <QWindow>
#include "NativeWindowFilter.h"

namespace asclepios::gui
{
	class NativeWindowFilterPrivate
	{
	public:
		static QScopedPointer<NativeWindowFilter> instance;
		static QHash<NativeWindowHelper*, WId> windows;
		static QHash<QWindow*, WId> winIds;
		static QHash<WId, NativeWindowHelper*> helpers;
	};
}
