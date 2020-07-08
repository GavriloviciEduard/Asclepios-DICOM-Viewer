#pragma once

#include "ui_layoutMenu.h"

namespace asclepios::gui
{
	class LayoutMenu : public QMenu
	{
	Q_OBJECT

	public:
		explicit LayoutMenu(QWidget* parent = Q_NULLPTR);
		~LayoutMenu() = default;

	private:
		Ui::layoutMenu m_ui = {};

		void initView();
	};
}
