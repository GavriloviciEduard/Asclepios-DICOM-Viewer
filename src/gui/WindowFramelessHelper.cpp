#include "WindowFramelessHelper.h"
#include "WindowFramelessHelper_p.h"

#include <QtQuick/QQuickWindow>

// class WindowFramelessHelper

asclepios::gui::WindowFramelessHelper::WindowFramelessHelper(QObject* parent)
	: QObject(parent)
	  , d_ptr(new WindowFramelessHelperPrivate())
{
}

asclepios::gui::WindowFramelessHelper::~WindowFramelessHelper()
= default;

void asclepios::gui::WindowFramelessHelper::classBegin()
{
}

void asclepios::gui::WindowFramelessHelper::componentComplete()
{
	Q_D(WindowFramelessHelper);

	auto obj = parent();
	while (nullptr != obj)
	{
		if (obj->inherits("QQuickRootItem"))
		{
			if (const auto rootItem = qobject_cast<QQuickItem*>(obj))
			{
				if (const auto window = rootItem->window())
				{
					d->window = window;

					d->helper = new NativeWindowHelper(window, d);
					connect(d->helper, &NativeWindowHelper::scaleFactorChanged,
					        this, &WindowFramelessHelper::scaleFactorChanged);
					if (!qFuzzyCompare(d->helper->scaleFactor(), 1.0))
					{
						emit scaleFactorChanged();
					}
				}

				break;
			}
		}

		obj = obj->parent();
	}
}

void asclepios::gui::WindowFramelessHelper::setTopDraggableMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priDraggableMargins.top())
	{
		d->priDraggableMargins.setTop(dm);
		emit topDraggableMarginChanged();
	}
}

void asclepios::gui::WindowFramelessHelper::setLeftDraggableMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priDraggableMargins.left())
	{
		d->priDraggableMargins.setLeft(dm);
		emit leftDraggableMarginChanged();
	}
}

void asclepios::gui::WindowFramelessHelper::setRightDraggableMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priDraggableMargins.right())
	{
		d->priDraggableMargins.setRight(dm);
		emit rightDraggableMarginChanged();
	}
}

void asclepios::gui::WindowFramelessHelper::setBottomDraggableMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priDraggableMargins.bottom())
	{
		d->priDraggableMargins.setBottom(dm);
		emit bottomDraggableMarginChanged();
	}
}

int asclepios::gui::WindowFramelessHelper::topDraggableMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priDraggableMargins.top();
}

int asclepios::gui::WindowFramelessHelper::leftDraggableMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priDraggableMargins.left();
}

int asclepios::gui::WindowFramelessHelper::rightDraggableMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priDraggableMargins.right();
}

int asclepios::gui::WindowFramelessHelper::bottomDraggableMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priDraggableMargins.bottom();
}

void asclepios::gui::WindowFramelessHelper::setTopMaximizedMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priMaximizedMargins.top())
	{
		d->priMaximizedMargins.setTop(dm);
		emit topMaximizedMarginChanged();
	}
}

void asclepios::gui::WindowFramelessHelper::setLeftMaximizedMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priMaximizedMargins.left())
	{
		d->priMaximizedMargins.setLeft(dm);
		emit leftMaximizedMarginChanged();
	}
}

void asclepios::gui::WindowFramelessHelper::setRightMaximizedMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priMaximizedMargins.right())
	{
		d->priMaximizedMargins.setRight(dm);
		emit rightMaximizedMarginChanged();
	}
}

void asclepios::gui::WindowFramelessHelper::setBottomMaximizedMargin(int dm)
{
	Q_D(WindowFramelessHelper);

	if (dm != d->priMaximizedMargins.bottom())
	{
		d->priMaximizedMargins.setBottom(dm);
		emit bottomMaximizedMarginChanged();
	}
}

int asclepios::gui::WindowFramelessHelper::topMaximizedMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priMaximizedMargins.top();
}

int asclepios::gui::WindowFramelessHelper::leftMaximizedMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priMaximizedMargins.left();
}

int asclepios::gui::WindowFramelessHelper::rightMaximizedMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priMaximizedMargins.right();
}

int asclepios::gui::WindowFramelessHelper::bottomMaximizedMargin() const
{
	Q_D(const WindowFramelessHelper);

	return d->priMaximizedMargins.bottom();
}

void asclepios::gui::WindowFramelessHelper::addIncludeItem(QQuickItem* item)
{
	Q_D(WindowFramelessHelper);

	d->includeItems.insert(item);
}

void asclepios::gui::WindowFramelessHelper::removeIncludeItem(QQuickItem* item)
{
	Q_D(WindowFramelessHelper);

	d->includeItems.remove(item);
}

void asclepios::gui::WindowFramelessHelper::addExcludeItem(QQuickItem* item)
{
	Q_D(WindowFramelessHelper);

	d->excludeItems.insert(item);
}

void asclepios::gui::WindowFramelessHelper::removeExcludeItem(QQuickItem* item)
{
	Q_D(WindowFramelessHelper);

	d->excludeItems.remove(item);
}

void asclepios::gui::WindowFramelessHelper::setTitleBarHeight(int value)
{
	Q_D(WindowFramelessHelper);

	if (value != d->titleBarHeight)
	{
		d->titleBarHeight = value;
		emit titleBarHeightChanged();
	}
}

int asclepios::gui::WindowFramelessHelper::titleBarHeight() const
{
	Q_D(const WindowFramelessHelper);

	return d->titleBarHeight;
}

qreal asclepios::gui::WindowFramelessHelper::scaleFactor() const
{
	Q_D(const WindowFramelessHelper);

	return d->helper ? d->helper->scaleFactor() : 1.0;
}

void asclepios::gui::WindowFramelessHelper::triggerMinimizeButtonAction()
{
	Q_D(WindowFramelessHelper);

	if (d->window)
	{
		const auto oldStates = d->window->windowStates();
		d->window->setWindowStates((oldStates & ~Qt::WindowActive) | Qt::WindowMinimized);
	}
}

void asclepios::gui::WindowFramelessHelper::triggerMaximizeButtonAction()
{
	Q_D(WindowFramelessHelper);

	if (d->window)
	{
		if (QWindow::Maximized == d->window->visibility())
		{
			d->window->showNormal();
		}
		else
		{
			d->window->showMaximized();
		}
	}
}

void asclepios::gui::WindowFramelessHelper::triggerCloseButtonAction()
{
	Q_D(WindowFramelessHelper);

	if (d->window)
	{
		d->window->close();
	}
}

// class WindowFramelessHelperPrivate

asclepios::gui::WindowFramelessHelperPrivate::WindowFramelessHelperPrivate()
	: window(nullptr)
	  , helper(nullptr)
	  , titleBarHeight(0)
{
}

asclepios::gui::WindowFramelessHelperPrivate::~WindowFramelessHelperPrivate()
{
}

QMargins asclepios::gui::WindowFramelessHelperPrivate::draggableMargins() const
{
	return priDraggableMargins;
}

QMargins asclepios::gui::WindowFramelessHelperPrivate::maximizedMargins() const
{
	return priMaximizedMargins;
}

bool asclepios::gui::WindowFramelessHelperPrivate::hitTest(const QPoint& pos) const
{
	const int scaledTitleBarHeight = titleBarHeight * helper->scaleFactor();

	if (!window)
		return false;
	if (scaledTitleBarHeight == 0)
		return false;
	if ((scaledTitleBarHeight > 0)
		&& (pos.y() >= scaledTitleBarHeight))
		return false;

	const auto rootItem = window->contentItem();
	if (nullptr == rootItem)
	{
		return false;
	}

	int currentIndex = -1;
	QQuickItem* items[32] = {nullptr};
	auto child = rootItem;
	QPointF p = pos;

	while (child && (currentIndex < 31))
	{
		items[++currentIndex] = child;
		const auto grandchild = child->childAt(p.x(), p.y());
		if (nullptr == grandchild)
		{
			break;
		}

		p = child->mapToItem(grandchild, p);
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
		if (rootItem == child)
		{
			break;
		}
	}

	return true;
}
