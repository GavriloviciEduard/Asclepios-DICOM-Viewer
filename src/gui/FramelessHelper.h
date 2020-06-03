#pragma once

#include <QWidget>

namespace asclepios::gui
{
	class FramelessHelperPrivate;

	class FramelessHelper : public QObject
	{
	Q_OBJECT

		Q_DECLARE_PRIVATE(FramelessHelper)
	public:
		explicit FramelessHelper(QWidget* parent = nullptr);
		virtual ~FramelessHelper();

		void setDraggableMargins(int left, int top, int right, int bottom);
		void setMaximizedMargins(int left, int top, int right, int bottom);
		void setDraggableMargins(const QMargins& margins);
		void setMaximizedMargins(const QMargins& margins);
		[[nodiscard]] QMargins draggableMargins() const;
		[[nodiscard]] QMargins maximizedMargins() const;

		void addIncludeItem(QWidget* item);
		void removeIncludeItem(QWidget* item);
		void addExcludeItem(QWidget* item);
		void removeExcludeItem(QWidget* item);

	signals:
		void titleBarHeightChanged(int newValue);
	public slots:
		void setTitleBarHeight(int value);
	public:
		[[nodiscard]] int titleBarHeight() const;

	signals:
		void scaleFactorChanged(qreal factor);
	public:
		[[nodiscard]] qreal scaleFactor() const;

	signals:
		void maximizedChanged(bool maximized);
	public:
		[[nodiscard]] bool isMaximized() const;

	public slots:
		void triggerMinimizeButtonAction();
		void triggerMaximizeButtonAction();
		void triggerCloseButtonAction();

	protected:
		bool eventFilter(QObject* obj, QEvent* ev) final;
		QScopedPointer<FramelessHelperPrivate> d_ptr;
	};
}
