#pragma once

#include "ui_guiframe.h"
#include "FramelessHelper.h"

namespace asclepios::gui
{
	class GUIFrame final : public QFrame
	{
	Q_OBJECT

	public:
		explicit GUIFrame(QWidget* parent = Q_NULLPTR);
		~GUIFrame() = default;

		void setContent(QWidget* t_widget);

	protected:
		void updateMaximizeButton(const bool& maximized) const;
		void moveEvent(QMoveEvent* event) override;

	private:
		Ui::GUIFrame m_ui = {};
		QWidget* m_childWidget = {};
		std::unique_ptr<FramelessHelper> m_helper = {};
		unsigned short m_currentScreen;

		void initView();
		void createConnections() const;
		void setUpFramelessHelper();
	};
}
