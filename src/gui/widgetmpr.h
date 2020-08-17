#pragma once

#include <QFuture>
#include <QVTKOpenGLNativeWidget.h>
#include "ui_widgetmpr.h"
#include "vtkwidgetmpr.h"
#include "toolbarwidgetmpr.h"

namespace asclepios::gui
{
	class WidgetMPR final : public WidgetBase
	{
	Q_OBJECT
	public:
		explicit WidgetMPR(QWidget* parent = Q_NULLPTR);
		~WidgetMPR() { m_future.waitForFinished(); }

		//getters
		[[nodiscard]] QFuture<void> getFuture() const { return m_future; }

		void render() override;

	signals:
		void finishedRenderAsync();

	private slots:
		void onActivateResliceWidget(const bool& t_flag) const;
		void onResetResliceWidget() const;
		void onSetMaximized() const;
		void onActivateWidget(const bool& t_flag, QObject* t_object);
		void onFinishedRenderAsync();

	private:
		Ui::WidgetMPR m_ui = {};
		QVTKOpenGLNativeWidget* m_qtvtkWidgets[3] = {};
		std::unique_ptr<vtkWidgetMPR> m_widgetMPR = {};
		ToolbarWidgetMPR* m_toolbar = {};
		QFuture<void> m_future = {};


		void initData() override;
		void initView() override;
		void createConnections() override;
		void startLoadingAnimation() override;
		void static onRenderAsync(WidgetMPR* t_self);
	};
}
