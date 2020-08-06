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
		
		void initView() override;
		void initData() override;
		void render() override;
		void createConnections() override;
		void resetView() override;
		void setSliderValues(const int& t_min, const int& t_max, const int& t_value) override;

	signals:
		void imageReaderInitialized();
		
	public slots:
		void activateWidget(const bool& t_flag);
		void applyTransformation(const transformationType& t_type) const;
		void refreshScrollValues(core::Series* t_series, core::Image* t_image);
		void changeScrollValue(vtkObject* t_obj, unsigned long t_index, void*, void*) const;
		void setMaximized() const;

	private slots :
		void changeImage(int t_index);
		void renderFinished();

	protected:
		void closeEvent(QCloseEvent* t_event) override;

	private:
		Ui::Widget2D m_ui = {};
		QVTKOpenGLNativeWidget* m_qtvtkWidget = {};
		std::unique_ptr<vtkWidgetBase> m_vtkWidget = {};
		vtkSmartPointer<vtkEventQtSlotConnect> m_scrollConnection = {};
		QScrollBar* m_scroll = {};
		QFuture<void> m_future = {};

		void connectScroll();
		void startLoadingAnimation() override;
		void disconnectScroll() const;
		void resetWidgets();
		void resetScroll();
		void setScrollStyle() const;
		static void initImageReader(vtkWidget2D* t_vtkWidget2D, Widget2D* t_self);
		[[nodiscard]] bool canScrollBeRefreshed(const int& t_patientIndex, const int& t_studyIndex,
		                                        const int& t_seriesIndex) const;
	};
}
