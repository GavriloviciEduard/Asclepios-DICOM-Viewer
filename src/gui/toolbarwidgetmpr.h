#pragma once

#include <QWidget>
#include "ui_toolbarwidgetmpr.h"

namespace asclepios::gui
{
	class ToolbarWidgetMPR final : public QWidget
	{
	Q_OBJECT
	public:
		explicit ToolbarWidgetMPR(QWidget* parent = Q_NULLPTR);
		~ToolbarWidgetMPR() = default;

		//getters
		[[nodiscard]] Ui::ToolbarWidgetMPR getUI() const { return m_ui; }

	signals:
		void activateResliceWidget(const bool& t_flag);
		void resetResliceWidget();

	private slots:
		void onButtonPressed();

	private:
		Ui::ToolbarWidgetMPR m_ui = {};

		void initView();
		void createConnections() const;
	};
}
