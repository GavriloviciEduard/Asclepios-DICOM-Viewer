#pragma once

#include <memory>
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

		void onChangeLayout(const WidgetsContainer::layouts& t_layout) const;
		void onCloseAllPatients() const;
		void onOpenFile();
		void onOpenFolder();
		
	public slots:
		void onApplyTransformation(const transformationType& t_type) const;
		void onShowThumbnailsWidget(const bool& t_flag) const;
	
	private:
		Ui::guiClass m_ui = {};
		std::shared_ptr<FilesImporter> m_filesImporter = {};
		std::unique_ptr<WidgetsController> m_widgetsController = {};
		ThumbnailsWidget* m_thumbnailsWidget = {};

		void initView();
		void initData();
		void createConnections() const;
		void connectFilesImporter() const;
		void disconnectFilesImporter() const;
		void connectFunctions() const;
	};
}
