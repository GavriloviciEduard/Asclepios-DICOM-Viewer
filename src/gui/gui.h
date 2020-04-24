#pragma once

#include "ui_gui.h"
#include "corecontroller.h"
#include "FramelessHelper.h"

namespace asclepios::gui
{
	class GUI final : public QMainWindow
	{
	Q_OBJECT

	public:
		explicit GUI(QWidget* parent = Q_NULLPTR);


	private slots:
		void openFileClicked();
		void openFolderClicked();

	protected:
		void updateMaximizeButton(const bool& maximized) const;
		void paintEvent(QPaintEvent* event) override;

	private:
		Ui::guiClass m_ui = {};
		std::unique_ptr<FramelessHelper> m_helper = {};
		std::unique_ptr<core::CoreController> m_coreController = {};

		void initView();
		void initData();
		void setUpFramelessHelper();
		void createConnections() const;
	};
}
