#pragma once

#include <QMenu>
#include "ui_filemenu.h"

namespace asclepios::gui
{
	class FileMenu final : public QMenu
	{
		Q_OBJECT

	public:
		explicit FileMenu(QWidget* parent = Q_NULLPTR);
		~FileMenu() = default;

	private slots:
		void onActionTriggered(QAction* t_action);

	signals:
		void openFile();
		void openFolder();
		void closeAllPatients();

	private:
		Ui::FileMenu m_ui = {};

		void initView();
		void createConnections(QWidget* parent) const;
	};
}

