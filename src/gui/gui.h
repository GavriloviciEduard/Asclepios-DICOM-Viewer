#pragma once

#include "ui_gui.h"
#include "corecontroller.h"
#include "thumbnailswidget.h"
#include "buttonswidget.h"

namespace asclepios::gui
{
	class GUI final : public QMainWindow
	{
	Q_OBJECT

	public:
		explicit GUI(QWidget* parent = Q_NULLPTR);

	private:
		Ui::guiClass m_ui = {};
		std::unique_ptr<core::CoreController> m_coreController = {};
		std::unique_ptr<ThumbnailsWidget> m_thumbnailsWidget = {};
		std::unique_ptr<ButtonsWidget> m_buttonsWidget = {};

		void initView();
		void initData();
		void createConnections() const;
	};
}
