#pragma once

#include "filemenu.h"
#include "framelesswindow.h"
#include "layoutmenu.h"
#include "ui_guiframe.h"

namespace asclepios::gui
{
	class GUIFrame final : public Frameless
	{
	Q_OBJECT
	public:
		explicit GUIFrame(QWidget* parent = Q_NULLPTR);
		~GUIFrame() = default;

		void setContent(QWidget* t_widget);

	public slots:
		void onChangeLayout(const WidgetsContainer::layouts& t_layout) const;
		void onOpenFile() const;
		void onOpenFolder() const;
		void onCloseAllPatients() const;

	protected:
		void updateMaximizeButton(const bool& maximized) const;
		void changeEvent(QEvent* t_event) override;

	private slots:
		void onClose();
		void onMaximize();
		void onMinimize();

	private:
		Ui::GUIFrame m_ui = {};
		QWidget* m_childWidget = {};
		LayoutMenu* m_layoutMenu = {};
		FileMenu* m_fileMenu = {};

		
		void initView();
		void createMenuBar();
	};
}
