#pragma once

#include "ui_tabwidget.h"
#include "widgetbase.h"

namespace asclepios::gui
{
	class TabWidget final : public QWidget
	{
	Q_OBJECT
	public:
		explicit TabWidget(QWidget* parent);
		~TabWidget() = default;

		void createWidget2D();
		void resetWidget() const;

		//getters
		[[nodiscard]] WidgetBase::WidgetType getWidgetType() const;
		[[nodiscard]] WidgetBase* getTabbedWidget() const { return m_tabbedWidget; }
		[[nodiscard]] WidgetBase* getActiveTabbedWidget() const;
		[[nodiscard]] bool getIsActive() const { return m_isActive; }
		[[nodiscard]] bool getIsMaximized() const { return m_isMaximized; }

		//setters
		void setIsActive(const bool& t_flag) { m_isActive = t_flag; }
		void setIsMaximized(const bool& t_flag) { m_isMaximized = t_flag; }
		void setTabTitle(const int& t_index, const QString& t_name) const;

	public slots:
		void onFocus(const bool& t_flag);
		void onMaximize();
		
	signals:
		void focused(TabWidget* t_widget);
		void setMaximized(TabWidget* t_widget);

	protected:
		void focusInEvent(QFocusEvent* event) override;

	private:
		Ui::TabWidget m_ui = {};
		WidgetBase* m_tabbedWidget = {};
		bool m_isActive = false;
		bool m_isMaximized = false;
	};
}
