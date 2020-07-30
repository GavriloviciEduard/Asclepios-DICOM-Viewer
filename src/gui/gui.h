#pragma once

#include <memory>
#include "filesimporter.h"
#include "imagefunctionstoolbar.h"
#include "layoutmenu.h"
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

	public slots:
		void onChangeLayout(const WidgetsContainer::layouts& t_layout) const;

	private:
		Ui::guiClass m_ui = {};
		std::shared_ptr<FilesImporter> m_filesImporter = {};
		std::unique_ptr<WidgetsController> m_widgetsController = {};
		ThumbnailsWidget* m_thumbnailsWidget = {};
		ImageFunctionsToolbar* m_imageFunctionsTtoolbar = {};
		LayoutMenu* m_layoutMenu = {};

		void initView();
		void initData();
		void createConnections() const;
		void connectGUIActions() const;
		void connectFilesImporter() const;
	};
}
