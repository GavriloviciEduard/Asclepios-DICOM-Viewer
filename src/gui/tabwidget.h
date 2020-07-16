#pragma once

#include "ui_tabwidget.h"
#include "WidgetBase.h"

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

		//setters
		void setIsActive(const bool& t_flag) { m_isActive = t_flag; }


	private:
		Ui::TabWidget m_ui = {};
		WidgetBase* m_tabbedWidget = {};
		bool m_isActive = false;
	};
}
