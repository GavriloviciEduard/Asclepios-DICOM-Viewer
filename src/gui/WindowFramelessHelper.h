#pragma once

#include <QtQuick/QQuickItem>
#include <QObject>

namespace asclepios::gui
{
	class WindowFramelessHelperPrivate;

	class WindowFramelessHelper : public QObject, public QQmlParserStatus
	{
	Q_OBJECT
		Q_DECLARE_PRIVATE(WindowFramelessHelper)

		Q_PROPERTY(
			int topDraggableMargin READ topDraggableMargin WRITE setTopDraggableMargin NOTIFY topDraggableMarginChanged)
		Q_PROPERTY(
			int leftDraggableMargin READ leftDraggableMargin WRITE setLeftDraggableMargin NOTIFY
			leftDraggableMarginChanged)
		Q_PROPERTY(
			int rightDraggableMargin READ rightDraggableMargin WRITE setRightDraggableMargin NOTIFY
			rightDraggableMarginChanged)
		Q_PROPERTY(
			int bottomDraggableMargin READ bottomDraggableMargin WRITE setBottomDraggableMargin NOTIFY
			bottomDraggableMarginChanged)

		Q_PROPERTY(
			int topMaximizedMargin READ topMaximizedMargin WRITE setTopMaximizedMargin NOTIFY topMaximizedMarginChanged)
		Q_PROPERTY(
			int leftMaximizedMargin READ leftMaximizedMargin WRITE setLeftMaximizedMargin NOTIFY
			leftMaximizedMarginChanged)
		Q_PROPERTY(
			int rightMaximizedMargin READ rightMaximizedMargin WRITE setRightMaximizedMargin NOTIFY
			rightMaximizedMarginChanged)
		Q_PROPERTY(
			int bottomMaximizedMargin READ bottomMaximizedMargin WRITE setBottomMaximizedMargin NOTIFY
			bottomMaximizedMarginChanged)

		Q_PROPERTY(int titleBarHeight READ titleBarHeight WRITE setTitleBarHeight NOTIFY titleBarHeightChanged)

		Q_PROPERTY(qreal scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)

		Q_INTERFACES(QQmlParserStatus)

	public:
		explicit WindowFramelessHelper(QObject* parent = nullptr);
		virtual ~WindowFramelessHelper();

		void classBegin() final;
		void componentComplete() final;

	signals:
		void topDraggableMarginChanged();
		void leftDraggableMarginChanged();
		void rightDraggableMarginChanged();
		void bottomDraggableMarginChanged();
	public:
		void setTopDraggableMargin(int dm);
		void setLeftDraggableMargin(int dm);
		void setRightDraggableMargin(int dm);
		void setBottomDraggableMargin(int dm);
		[[nodiscard]] int topDraggableMargin() const;
		[[nodiscard]] int leftDraggableMargin() const;
		[[nodiscard]] int rightDraggableMargin() const;
		[[nodiscard]] int bottomDraggableMargin() const;

	signals:
		void topMaximizedMarginChanged();
		void leftMaximizedMarginChanged();
		void rightMaximizedMarginChanged();
		void bottomMaximizedMarginChanged();
	public:
		void setTopMaximizedMargin(int dm);
		void setLeftMaximizedMargin(int dm);
		void setRightMaximizedMargin(int dm);
		void setBottomMaximizedMargin(int dm);
		[[nodiscard]] int topMaximizedMargin() const;
		[[nodiscard]] int leftMaximizedMargin() const;
		[[nodiscard]] int rightMaximizedMargin() const;
		[[nodiscard]] int bottomMaximizedMargin() const;

	public slots:
		void addIncludeItem(QQuickItem* item);
		void removeIncludeItem(QQuickItem* item);
	public slots:
		void addExcludeItem(QQuickItem* item);
		void removeExcludeItem(QQuickItem* item);

	signals:
		void titleBarHeightChanged();
	public slots:
		void setTitleBarHeight(int value);
	public:
		[[nodiscard]] int titleBarHeight() const;

	signals:
		void scaleFactorChanged();
	public:
		[[nodiscard]] qreal scaleFactor() const;

	public slots:
		void triggerMinimizeButtonAction();
		void triggerMaximizeButtonAction();
		void triggerCloseButtonAction();

	protected:
		QScopedPointer<WindowFramelessHelperPrivate> d_ptr;
	};
}
