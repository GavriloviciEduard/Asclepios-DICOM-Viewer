#pragma once

#include "ui_layoutMenu.h"
#include "widgetscontainer.h"

namespace asclepios::gui
{
	class LayoutMenu : public QMenu
	{
	Q_OBJECT

	public:
		explicit LayoutMenu(QWidget* parent = Q_NULLPTR);
		~LayoutMenu() = default;

	private slots:
		void onActionTriggered(QAction* t_action);

		signals:
			void changeLayout(const WidgetsContainer::layouts& t_layout);
		
	private:
		Ui::layoutMenu m_ui = {};

		void initView();
		void createConnections(QWidget* parent) const;
	};
}
