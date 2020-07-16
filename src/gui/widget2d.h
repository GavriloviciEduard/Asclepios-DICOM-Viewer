#pragma once

#include "ui_widget2d.h"
#include "widgetbase.h"
#include <QVTKOpenGLNativeWidget.h>

namespace asclepios::gui
{
	class Widget2D final : public WidgetBase
	{
	Q_OBJECT
	public:
		explicit Widget2D(QWidget* parent = Q_NULLPTR);
		~Widget2D();

		void initView() override;
		void render() override;
		void propagateFocusFromChild() override;
		void resetView() override;
		void setWindowLevel(const int& t_window, const int& t_level) override;
		void setSliderValues(const int& t_min, const int& t_max, const int& t_value) override;
		void focusInEvent(QFocusEvent* event) override;

	private:
		Ui::Widget2D m_ui;
		QVTKOpenGLNativeWidget* m_qtvtkWidget;
		//std::unique_ptr<vtkViewerWidget> m_vtkWidget;

	};
}
