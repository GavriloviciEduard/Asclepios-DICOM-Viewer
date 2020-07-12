#pragma once

#include <QTabWidget>
#include "ui_thumbnailswidget.h"


namespace asclepios::core
{
	class Patient;
	class Study;
	class Series;
	class Image;
}

namespace asclepios::gui
{
	class ThumbnailsWidget final : public QWidget
	{
	Q_OBJECT

	public:
		explicit ThumbnailsWidget(QWidget* parent = Q_NULLPTR);
		~ThumbnailsWidget() = default;

	public slots:
		void addThumbnail(core::Patient* t_patient,
		                  core::Study* t_study,
		                  core::Series* t_series,
		                  core::Image* t_image) const;

	private:
		Ui::ThumbnailsWidget m_ui = {};
		QTabWidget* m_patientsTabs = {};

		void initView();
		void initData();
		[[nodiscard]] bool tryInsertExistingItem(core::Patient* t_patient,
		                                         core::Study* t_study,
		                                         core::Series* t_series,
		                                         core::Image* t_image) const;
		void insertNewItem(core::Patient* t_patient,
		                   core::Study* t_study,
		                   core::Series* t_series,
		                   core::Image* t_image) const;
	};
}
