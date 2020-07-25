#pragma once

#include <qfuture.h>
#include <qscrollbar.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkEventQtSlotConnect.h>
#include "ui_widget2d.h"
#include "vtkwidgetbase.h"
#include "widgetbase.h"

namespace asclepios::gui
{
	class Widget2D final : public WidgetBase
	{
	Q_OBJECT
	public:
		explicit Widget2D(QWidget* parent = Q_NULLPTR);
		~Widget2D();

		void initView() override;
		void initData() override;
		void render() override;
		void createActivationConnections() override;
		void resetView() override;
		void setWindowLevel(const int& t_window, const int& t_level) override;
		void setSliderValues(const int& t_min, const int& t_max, const int& t_value) override;
		void focusInEvent(QFocusEvent* event) override;

	public slots:
		void activateWidget(const bool& t_flag);

	private:
		Ui::Widget2D m_ui = {};
		QVTKOpenGLNativeWidget* m_qtvtkWidget = {};
		std::unique_ptr<vtkWidgetBase> m_vtkWidget;
		QScrollBar* m_scroll = {};
		vtkSmartPointer<vtkEventQtSlotConnect> m_scrollConnections = {};
		QFuture<void> m_future = {};
	};
}
