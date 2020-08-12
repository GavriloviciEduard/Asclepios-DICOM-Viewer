#pragma once

#include <qfuture.h>
#include <qscrollbar.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkEventQtSlotConnect.h>
#include "ui_widget2d.h"
#include "vtkwidget2d.h"
#include "vtkwidgetbase.h"
#include "widgetbase.h"



namespace asclepios::gui
{
	class Widget2D final : public WidgetBase
	{
	Q_OBJECT
	public:
		explicit Widget2D(QWidget* parent = Q_NULLPTR);
		~Widget2D() = default;

		//getters
		[[nodiscard]] QVTKOpenGLNativeWidget* getWidgetOpenGLNative() const { return m_qtvtkWidget; }
		[[nodiscard]] vtkWidgetBase* getWidgetVTK() const { return m_vtkWidget.get(); }
		[[nodiscard]] QScrollBar* getScrollBar() const { return m_scroll; }
		[[nodiscard]] QFuture<void> getFuture() const { return m_future; }
		
		void render() override;
		
	signals:
		void imageReaderInitialized();
		
	public slots:
		void onActivateWidget(const bool& t_flag);
		void onApplyTransformation(const transformationType& t_type) const;
		void onRefreshScrollValues(core::Series* t_series, core::Image* t_image);
		void onChangeScrollValue(vtkObject* t_obj, unsigned long t_index, void*, void*) const;
		void onSetMaximized() const;

	private slots :
		void onChangeImage(int t_index);
		void onRenderFinished();

	protected:
		void closeEvent(QCloseEvent* t_event) override;

	private:
		Ui::Widget2D m_ui = {};
		QVTKOpenGLNativeWidget* m_qtvtkWidget = {};
		std::unique_ptr<vtkWidgetBase> m_vtkWidget = {};
		vtkSmartPointer<vtkEventQtSlotConnect> m_scrollConnection = {};
		QScrollBar* m_scroll = {};
		QFuture<void> m_future = {};

		void initView() override;
		void initData() override;
		void createConnections() override;
		void resetView() override;
		void connectScroll();
		void startLoadingAnimation() override;
		void disconnectScroll() const;
		void resetWidgets();
		void resetScroll();
		void setScrollStyle() const;
		void setSliderValues(const int& t_min, const int& t_max, const int& t_value) override;
		static void initImageReader(vtkWidget2D* t_vtkWidget2D, Widget2D* t_self);
		[[nodiscard]] bool canScrollBeRefreshed(const int& t_patientIndex, const int& t_studyIndex,
		                                        const int& t_seriesIndex) const;
	};
}
