#pragma once

#include <QWidget>
#include "ui_buttonswidget.h"


namespace asclepios::gui
{
	class ButtonsWidget final : public QWidget
	{
	Q_OBJECT

	public:
		explicit ButtonsWidget(QWidget* parent = Q_NULLPTR);
		~ButtonsWidget() = default;

		//getters
		Ui::ButtonsWidget getUI() const { return m_ui; }

	private:
		Ui::ButtonsWidget m_ui = {};

		void initView();
	};
}
