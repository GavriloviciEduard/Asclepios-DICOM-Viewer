#pragma once

#include <QWindow>
#include <QMargins>

namespace asclepios::gui
{
	class NativeWindowTester
	{
	public:
		virtual ~NativeWindowTester() = default;
		virtual QMargins draggableMargins() const = 0;
		virtual QMargins maximizedMargins() const = 0;

		virtual bool hitTest(const QPoint& pos) const = 0;
	};

	class NativeWindowHelperPrivate;

	class NativeWindowHelper : public QObject
	{
	Q_OBJECT
		Q_DECLARE_PRIVATE(NativeWindowHelper)

	public:
		NativeWindowHelper(QWindow* window, NativeWindowTester* tester);
		explicit NativeWindowHelper(QWindow* window);
		~NativeWindowHelper();

		[[nodiscard]] bool nativeEventFilter(void* msg, long* result);
	protected:
		[[nodiscard]] bool eventFilter(QObject* obj, QEvent* ev) final;
		QScopedPointer<NativeWindowHelperPrivate> d_ptr;

	signals:
		void scaleFactorChanged(qreal factor);
	public:
		[[nodiscard]] qreal scaleFactor() const;
	};
}
