#pragma once

#include <QFrame>
#include <QMainWindow>
#include <QWidget>
#include <QList>
#include <QMargins>
#include <QRect>


namespace asclepios::gui
{
	class Frameless : public QFrame
	{
	Q_OBJECT
	public:
		explicit Frameless(QWidget* parent = nullptr);
		~Frameless() = default;

		void setResizeable(bool resizeable = true);
		void setResizeableAreaWidth(int width = 5);
		void setContsMargins(const QMargins& margins);
		void setContMargins(int left, int top, int right, int bottom);

		[[nodiscard]] bool isResizeable() const { return m_bResizeable; }
		[[nodiscard]] QMargins contMargins() const;
		[[nodiscard]] QRect contRect() const;
		void getaContMargins(int* left, int* top, int* right, int* bottom) const;

	public slots:
		void fullScreen();

	protected:
		void setTitleBar(QWidget* titlebar);
		void ignoreWidget(QWidget* widget);
		bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	private slots:
		void onTitleBarDestroyed();

	private:
		QWidget* m_titlebar = {};
		QList<QWidget*> m_whiteList;
		int m_borderWidth;
		QMargins m_margins;
		QMargins m_frames;
		bool m_bJustMaximized;
		bool m_bResizeable;
	};
}
