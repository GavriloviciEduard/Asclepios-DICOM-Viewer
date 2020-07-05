#pragma once

#include <QWidget>
#include "ui_buttonswidget.h"


namespace asclepios::gui
{
	class ButtonsWidget : public QWidget
	{
	Q_OBJECT

	public:
		explicit ButtonsWidget(QWidget* parent = Q_NULLPTR);
		~ButtonsWidget() = default;

	private:
		Ui::ButtonsWidget m_ui = {};

		void initView();
	};
}
