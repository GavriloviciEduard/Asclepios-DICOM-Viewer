#include "FramelessHelper.h"
#include "FramelessHelper_p.h"
#include <windows.h>

// class FramelessHelper

asclepios::gui::FramelessHelper::FramelessHelper(QWidget* parent)
	: QObject(parent)
	  , d_ptr(new FramelessHelperPrivate())
{
	Q_D(FramelessHelper);
	d->window = parent;
	Q_CHECK_PTR(parent);

	if (d->window)
		d->window->installEventFilter(this);
}

asclepios::gui::FramelessHelper::~FramelessHelper()
= default;

void asclepios::gui::FramelessHelper::setDraggableMargins(const int left, const int top, const int right,
                                                          const int bottom)
{
	Q_D(FramelessHelper);

	d->priDraggableMargins = QMargins(left, top, right, bottom);
}

void asclepios::gui::FramelessHelper::setMaximizedMargins(const int left, const int top, const int right,
                                                          const int bottom)
{
	Q_D(FramelessHelper);

	d->priMaximizedMargins = QMargins(left, top, right, bottom);
}

void asclepios::gui::FramelessHelper::setDraggableMargins(const QMargins& margins)
{
	Q_D(FramelessHelper);

	d->priDraggableMargins = margins;
}

void asclepios::gui::FramelessHelper::setMaximizedMargins(const QMargins& margins)
{
	Q_D(FramelessHelper);

	d->priMaximizedMargins = margins;
}

QMargins asclepios::gui::FramelessHelper::draggableMargins() const
{
	Q_D(const FramelessHelper);

	return d->priDraggableMargins;
}

QMargins asclepios::gui::FramelessHelper::maximizedMargins() const
{
	Q_D(const FramelessHelper);

	return d->priMaximizedMargins;
}

void asclepios::gui::FramelessHelper::addIncludeItem(QWidget* item)
{
	Q_D(FramelessHelper);

	d->includeItems.insert(item);
}

void asclepios::gui::FramelessHelper::removeIncludeItem(QWidget* item)
{
	Q_D(FramelessHelper);

	d->includeItems.remove(item);
}

void asclepios::gui::FramelessHelper::addExcludeItem(QWidget* item)
{
	Q_D(FramelessHelper);

	d->excludeItems.insert(item);
}

void asclepios::gui::FramelessHelper::removeExcludeItem(QWidget* item)
{
	Q_D(FramelessHelper);

	d->excludeItems.remove(item);
}

void asclepios::gui::FramelessHelper::setTitleBarHeight(const int value)
{
	Q_D(FramelessHelper);

	if (value != d->titleBarHeight)
	{
		d->titleBarHeight = value;
		emit titleBarHeightChanged(value);
	}
}

int asclepios::gui::FramelessHelper::titleBarHeight() const
{
	Q_D(const FramelessHelper);

	return d->titleBarHeight;
}

qreal asclepios::gui::FramelessHelper::scaleFactor() const
{
	Q_D(const FramelessHelper);

	return d->helper ? d->helper->scaleFactor() : 1.0;
}

bool asclepios::gui::FramelessHelper::isMaximized() const
{
	Q_D(const FramelessHelper);

	return d->maximized;
}

void asclepios::gui::FramelessHelper::triggerMinimizeButtonAction()
{
	Q_D(FramelessHelper);

	if (d->window)
	{
		d->window->showMinimized();
	}
}

void asclepios::gui::FramelessHelper::triggerMaximizeButtonAction()
{
	Q_D(FramelessHelper);

	if (d->window)
	{
		if (d->window->windowState() & Qt::WindowMaximized)
		{
			d->window->showNormal();
		}
		else
		{
			d->window->showMaximized();
		}
	}
}

void asclepios::gui::FramelessHelper::triggerCloseButtonAction()
{
	Q_D(FramelessHelper);

	if (d->window)
	{
		d->window->close();
	}
}

bool asclepios::gui::FramelessHelper::eventFilter(QObject* obj, QEvent* ev)
{
	Q_D(FramelessHelper);

	if (ev->type() == QEvent::WindowStateChange)
	{
		const bool maximized = d->window->windowState() & Qt::WindowMaximized;
		if (maximized != d->maximized)
		{
			d->maximized = maximized;
			emit maximizedChanged(maximized);
		}
	}
	else if (ev->type() == QEvent::WinIdChange)
	{
		if (nullptr == d->helper)
		{
			const auto w = d->window->windowHandle();

			d->helper = new NativeWindowHelper(w, d);
			connect(d->helper, &NativeWindowHelper::scaleFactorChanged,
			        this, &FramelessHelper::scaleFactorChanged);
			if (!qFuzzyCompare(d->helper->scaleFactor(), 1.0))
			{
				emit scaleFactorChanged(d->helper->scaleFactor());
			}
		}
	}

#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    if ((ev->type() == QEvent::Resize) || (ev->type() == QEvent::WindowStateChange)) {
        if (d->window->windowState() & Qt::WindowMaximized) {
            const QMargins &m = d->priMaximizedMargins;
            int r = GetSystemMetrics(SM_CXFRAME) * 2 - m.left() - m.right();
            int b = GetSystemMetrics(SM_CYFRAME) * 2 - m.top() - m.bottom();

            d->window->setContentsMargins(0, 0, r, b);
        } else {
            d->window->setContentsMargins(0 ,0, 0, 0);
        }
    }
#endif

	return QObject::eventFilter(obj, ev);
}

// class FramelessHelperPrivate

asclepios::gui::FramelessHelperPrivate::FramelessHelperPrivate()
	: window(nullptr)
	  , helper(nullptr)
	  , titleBarHeight(0)
	  , maximized(false)
{
}

asclepios::gui::FramelessHelperPrivate::~FramelessHelperPrivate()
= default;

QMargins asclepios::gui::FramelessHelperPrivate::draggableMargins() const
{
	return priDraggableMargins;
}

QMargins asclepios::gui::FramelessHelperPrivate::maximizedMargins() const
{
	return priMaximizedMargins;
}

bool asclepios::gui::FramelessHelperPrivate::hitTest(const QPoint& pos) const
{
	const int scaledTitleBarHeight = static_cast<qreal>(titleBarHeight) * helper->scaleFactor();

	if (!window)
		return false;
	if (scaledTitleBarHeight == 0)
		return false;
	if (scaledTitleBarHeight > 0
		&& pos.y() >= scaledTitleBarHeight)
		return false;

	int currentIndex = -1;
	QWidget* items[32] = {nullptr};
	auto child = window;
	QPoint p = pos;

	while (child && (currentIndex < 31))
	{
		items[++currentIndex] = child;
		const auto grandchild = child->childAt(p);
		if (nullptr == grandchild)
		{
			break;
		}

		p = grandchild->mapFrom(child, p);
		child = grandchild;
	}

	while (currentIndex > 0)
	{
		child = items[currentIndex];
		--currentIndex;

		if (includeItems.contains(child))
		{
			break;
		}
		if (excludeItems.contains(child))
		{
			return false;
		}
		if (window == child)
		{
			break;
		}
	}

	return true;
}
