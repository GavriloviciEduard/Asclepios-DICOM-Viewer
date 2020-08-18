#pragma once

#include "ui_toolbarwidget3d.h"

namespace asclepios::gui
{
	class ToolbarWidget3D final : public QWidget
	{
	Q_OBJECT
	public:
		explicit ToolbarWidget3D(QWidget* parent = Q_NULLPTR);
		~ToolbarWidget3D() = default;

		[[nodiscard]] Ui::ToolbarWidget3D getUI() const { return m_ui; }

	private slots:
		void onFilterChanged(const int& t_index);
		void onCropPressed();

	signals:
		void filterChanged(const QString& t_path);
		void cropPressed(const bool& t_pressed);

	private:
		Ui::ToolbarWidget3D m_ui = {};

		void initView();
		void initCombo() const;
	};
}
