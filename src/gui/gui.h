#pragma once

#include <memory>
#include "buttonswidget.h"
#include "filesimporter.h"
#include "thumbnailswidget.h"
#include "ui_gui.h"
#include "widgetscontroller.h"

namespace asclepios::gui
{
	class GUI final : public QMainWindow
	{
	Q_OBJECT

	public:
		explicit GUI(QWidget* parent = Q_NULLPTR);
		~GUI();


	private slots:
		void onOpenFile();
		void onOpenFolder();

	private:
		Ui::guiClass m_ui = {};
		std::unique_ptr<FilesImporter> m_filesImporter = {};
		std::unique_ptr<WidgetsController> m_widgetsController = {};
		ThumbnailsWidget* m_thumbnailsWidget = {};
		ButtonsWidget* m_buttonsWidget = {};

		void initView();
		void initData();
		void createConnections() const;
		void connectGUIActions() const;
		void connectButtonsWidgetActions() const;
		void connectFilesImporter() const;
	};
}
