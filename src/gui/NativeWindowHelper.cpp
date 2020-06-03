#include "NativeWindowHelper.h"
#include "NativeWindowHelper_p.h"

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

#include <QScreen>
#include <QEvent>
#include <QtWin>

#include <QOperatingSystemVersion>

#include "NativeWindowFilter.h"

// class NativeWindowHelper

asclepios::gui::NativeWindowHelper::NativeWindowHelper(QWindow* window, asclepios::gui::NativeWindowTester* tester)
	: QObject(window)
	  , d_ptr(new asclepios::gui::NativeWindowHelperPrivate())
{
	d_ptr->q_ptr = this;

	Q_D(NativeWindowHelper);

	Q_CHECK_PTR(window);
	Q_CHECK_PTR(tester);

	d->window = window;
	d->tester = tester;

	if (d->window)
	{
		d->scaleFactor = d->window->screen()->devicePixelRatio();

		if (d->window->flags() & Qt::FramelessWindowHint)
		{
			d->window->installEventFilter(this);
			d->updateWindowStyle();
		}
	}
}

asclepios::gui::NativeWindowHelper::NativeWindowHelper(QWindow* window)
	: QObject(window)
	  , d_ptr(new NativeWindowHelperPrivate())
{
	d_ptr->q_ptr = this;

	Q_D(NativeWindowHelper);

	Q_CHECK_PTR(window);
	d->window = window;

	if (d->window)
	{
		d->scaleFactor = d->window->screen()->devicePixelRatio();

		if (d->window->flags() & Qt::FramelessWindowHint)
		{
			d->window->installEventFilter(this);
			d->updateWindowStyle();
		}
	}
}

asclepios::gui::NativeWindowHelper::~NativeWindowHelper()
= default;

bool asclepios::gui::NativeWindowHelper::nativeEventFilter(void* msg, long* result)
{
	Q_D(NativeWindowHelper);

	Q_CHECK_PTR(d->window);

	const auto lpMsg = reinterpret_cast<LPMSG>(msg);
	const WPARAM wParam = lpMsg->wParam;
	const LPARAM lParam = lpMsg->lParam;

	if (WM_NCHITTEST == lpMsg->message)
	{
		*result = d->hitTest(GET_X_LPARAM(lParam),
		                     GET_Y_LPARAM(lParam));
		return true;
	}
	else if (WM_NCACTIVATE == lpMsg->message)
	{
		if (!QtWin::isCompositionEnabled())
		{
			if (result) *result = 1;
			return true;
		}
	}
	else if (WM_NCCALCSIZE == lpMsg->message)
	{
		if (TRUE == wParam)
		{
			if (d->isMaximized())
			{
				NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

				const QRect g = d->availableGeometry();
				const QMargins m = d->maximizedMargins();

				params.rgrc[0].top = g.top() - m.top();
				params.rgrc[0].left = g.left() - m.left();
				params.rgrc[0].right = g.right() + m.right() + 1;
				params.rgrc[0].bottom = g.bottom() + m.bottom() + 1;
			}

			if (result) *result = 0;
			return true;
		}
	}
	else if (WM_GETMINMAXINFO == lpMsg->message)
	{
		const auto lpMinMaxInfo = reinterpret_cast<LPMINMAXINFO>(lParam);

		const QRect g = d->availableGeometry();
		const QMargins m = d->maximizedMargins();

		lpMinMaxInfo->ptMaxPosition.x = - m.left();
		lpMinMaxInfo->ptMaxPosition.y = - m.top();
		lpMinMaxInfo->ptMaxSize.x = g.right() - g.left() + 1 + m.left() + m.right();
		lpMinMaxInfo->ptMaxSize.y = g.bottom() - g.top() + 1 + m.top() + m.bottom();

		lpMinMaxInfo->ptMinTrackSize.x = d->window->minimumWidth();
		lpMinMaxInfo->ptMinTrackSize.y = d->window->minimumHeight();
		lpMinMaxInfo->ptMaxTrackSize.x = d->window->maximumWidth();
		lpMinMaxInfo->ptMaxTrackSize.y = d->window->maximumHeight();

		if (result) *result = 0;
		return true;
	}
	else if (WM_NCLBUTTONDBLCLK == lpMsg->message)
	{
		const auto minimumSize = d->window->minimumSize();
		const auto maximumSize = d->window->maximumSize();
		if ((minimumSize.width() >= maximumSize.width())
			|| (minimumSize.height() >= maximumSize.height()))
		{
			if (result) *result = 0;
			return true;
		}
	}
	else if (WM_DPICHANGED == lpMsg->message)
	{
		const qreal old = d->scaleFactor;
		if (HIWORD(wParam) < 144)
		{
			d->scaleFactor = 1.0;
		}
		else
		{
			d->scaleFactor = 2.0;
		}

		if (!qFuzzyCompare(old, d->scaleFactor))
		{
			emit scaleFactorChanged(d->scaleFactor);
		}

		const auto hWnd = reinterpret_cast<HWND>(d->window->winId());
		const auto rect = reinterpret_cast<LPRECT>(lParam);
		SetWindowPos(hWnd,
		             nullptr,
		             rect->left,
		             rect->top,
		             rect->right - rect->left,
		             rect->bottom - rect->top,
		             SWP_NOZORDER | SWP_NOACTIVATE);
	}

	return false;
}

bool asclepios::gui::NativeWindowHelper::eventFilter(QObject* obj, QEvent* ev)
{
	Q_D(NativeWindowHelper);

	if (ev->type() == QEvent::WinIdChange)
	{
		d->updateWindowStyle();
	}
	else if (ev->type() == QEvent::Show)
	{
		d->updateWindowStyle();
	}

	return QObject::eventFilter(obj, ev);
}

qreal asclepios::gui::NativeWindowHelper::scaleFactor() const
{
	Q_D(const NativeWindowHelper);

	return d->scaleFactor;
}

// class NativeWindowHelperPrivate

asclepios::gui::NativeWindowHelperPrivate::NativeWindowHelperPrivate()
	: q_ptr(nullptr)
	  , window(nullptr)
	  , tester(nullptr)
	  , oldWindow(nullptr)
	  , scaleFactor(1.0)
{
}

asclepios::gui::NativeWindowHelperPrivate::~NativeWindowHelperPrivate()
{
	if (window)
		NativeWindowFilter::deliver(window, nullptr);
}

void asclepios::gui::NativeWindowHelperPrivate::updateWindowStyle()
{
	Q_Q(NativeWindowHelper);

	Q_CHECK_PTR(window);

	const HWND hWnd = reinterpret_cast<HWND>(window->winId());
	if (nullptr == hWnd)
		return;
	if (oldWindow == hWnd)
	{
		return;
	}
	oldWindow = hWnd;

	NativeWindowFilter::deliver(window, q);

	const auto currentVersion = QOperatingSystemVersion::current();
	if (currentVersion < QOperatingSystemVersion::Windows8)
	{
		return;
	}

	const LONG oldStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME
		| WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;;
	LONG newStyle = WS_POPUP | WS_THICKFRAME;

	if (QtWin::isCompositionEnabled())
		newStyle |= WS_CAPTION;

	if (window->flags() & Qt::CustomizeWindowHint)
	{
		if (window->flags() & Qt::WindowSystemMenuHint)
			newStyle |= WS_SYSMENU;
		if (window->flags() & Qt::WindowMinimizeButtonHint)
			newStyle |= WS_MINIMIZEBOX;
		if (window->flags() & Qt::WindowMaximizeButtonHint)
			newStyle |= WS_MAXIMIZEBOX;
	}
	else
	{
		newStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}

	const LONG currentStyle = GetWindowLong(hWnd, GWL_STYLE);
	SetWindowLong(hWnd, GWL_STYLE, (currentStyle & ~oldStyle) | newStyle);

	SetWindowPos(hWnd, nullptr, 0, 0, 0, 0,
	             SWP_NOOWNERZORDER | SWP_NOZORDER |
	             SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

	if (QtWin::isCompositionEnabled())
		QtWin::extendFrameIntoClientArea(window, 1, 1, 1, 1);
}

int asclepios::gui::NativeWindowHelperPrivate::hitTest(int x, int y) const
{
	Q_CHECK_PTR(window);

	x = x / window->devicePixelRatio();
	y = y / window->devicePixelRatio();

	enum RegionMask
	{
		Client = 0x0000,
		Top = 0x0001,
		Left = 0x0010,
		Right = 0x0100,
		Bottom = 0x1000,
	};

	const auto wfg = window->frameGeometry();
	const QMargins draggableMargins
		= this->draggableMargins();

	int top = draggableMargins.top();
	int left = draggableMargins.left();
	int right = draggableMargins.right();
	int bottom = draggableMargins.bottom();

	if (top <= 0)
		top = GetSystemMetrics(SM_CYFRAME);
	if (left <= 0)
		left = GetSystemMetrics(SM_CXFRAME);
	if (right <= 0)
		right = GetSystemMetrics(SM_CXFRAME);
	if (bottom <= 0)
		bottom = GetSystemMetrics(SM_CYFRAME);

	const auto result =
		(Top * (y < (wfg.top() + top))) |
		(Left * (x < (wfg.left() + left))) |
		(Right * (x > (wfg.right() - right))) |
		(Bottom * (y > (wfg.bottom() - bottom)));

	const bool wResizable = window->minimumWidth() < window->maximumWidth();
	const bool hResizable = window->minimumHeight() < window->maximumHeight();

	switch (result)
	{
	case Top | Left: return wResizable && hResizable ? HTTOPLEFT : HTCLIENT;
	case Top: return hResizable ? HTTOP : HTCLIENT;
	case Top | Right: return wResizable && hResizable ? HTTOPRIGHT : HTCLIENT;
	case Right: return wResizable ? HTRIGHT : HTCLIENT;
	case Bottom | Right: return wResizable && hResizable ? HTBOTTOMRIGHT : HTCLIENT;
	case Bottom: return hResizable ? HTBOTTOM : HTCLIENT;
	case Bottom | Left: return wResizable && hResizable ? HTBOTTOMLEFT : HTCLIENT;
	case Left: return wResizable ? HTLEFT : HTCLIENT;
	default:
		const auto pos = window->mapFromGlobal(QPoint(x, y));
		return ((nullptr != tester) && !tester->hitTest(pos)) ? HTCLIENT : HTCAPTION;;
	}
}

bool asclepios::gui::NativeWindowHelperPrivate::isMaximized() const
{
	Q_CHECK_PTR(window);

	const HWND hWnd = reinterpret_cast<HWND>(window->winId());
	if (nullptr == hWnd)
		return false;

	WINDOWPLACEMENT windowPlacement;
	if (!GetWindowPlacement(hWnd, &windowPlacement))
		return false;

	return (SW_MAXIMIZE == windowPlacement.showCmd);
}

QMargins asclepios::gui::NativeWindowHelperPrivate::draggableMargins() const
{
	return tester ? tester->draggableMargins() * scaleFactor : QMargins();
}

QMargins asclepios::gui::NativeWindowHelperPrivate::maximizedMargins() const
{
	return tester ? tester->maximizedMargins() * scaleFactor : QMargins();
}

QRect asclepios::gui::NativeWindowHelperPrivate::availableGeometry() const
{
	MONITORINFO mi{0};
	mi.cbSize = sizeof(MONITORINFO);

	const auto hWnd = reinterpret_cast<HWND>(window->winId());
	auto hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	if (!hMonitor || !GetMonitorInfoW(hMonitor, &mi))
	{
		Q_ASSERT(NULL != hMonitor);
		return window->screen()->availableGeometry();
	}

	return QRect(mi.rcWork.left, mi.rcWork.top, mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top);
}
