#pragma once

#include <QWidget>
#include "ui_ThumbnailsWidget.h"
#include <QTabWidget>


namespace asclepios::gui
{
	class ThumbnailsWidget : public QWidget
	{
	Q_OBJECT

	public:
		explicit ThumbnailsWidget(QWidget* parent = Q_NULLPTR);
		~ThumbnailsWidget() = default;

	private:
		Ui::ThumbnailsWidget m_ui = {};
		QTabWidget* m_patientsTabs = {};

		void initView();
		void initData();
	};
}
