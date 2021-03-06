#pragma once

#include <QWidget>
#include <vtkRenderWindow.h>
#include "loadinganimation.h"
#include "vtkeventfilter.h"
#include "series.h"


namespace asclepios::gui
{
	class WidgetBase : public QWidget
	{
	Q_OBJECT
	public:
		explicit WidgetBase(QWidget* t_parent);
		~WidgetBase() = default;

		virtual void render() = 0;
			
		enum class WidgetType
		{
			none,
			widget2d,
			widgetmpr,
			widget3d
		};

		//getters
		[[nodiscard]] core::Series* getSeries() const { return m_series; }
		[[nodiscard]] core::Image* getImage() const { return m_image; }
		[[nodiscard]] bool getIsImageLoaded() const { return m_isImageLoaded; }
		[[nodiscard]] WidgetType getWidgetType() const { return m_widgetType; }
		[[nodiscard]] QWidget* getTabWidget() const { return m_tabWidget; }
		[[nodiscard]] vtkEventFilter* getvtkEventFilter() const { return m_vtkEvents.get(); }
		[[nodiscard]] int getPatientIndex() const { return m_patientIndex; }
		[[nodiscard]] int getStudyInex() const { return m_studyIndex; }
		[[nodiscard]] int getSeriesIndex() const { return m_seriesIndex; }
		[[nodiscard]] int getImageIndex() const { return m_imageIndex; }

		//setters
		void setSeries(core::Series* t_series) { m_series = t_series; }
		void setImage(core::Image* t_image) { m_image = t_image; }
		void setIsImageLoaded(const bool& t_flag) { m_isImageLoaded = t_flag; }
		void setWidgetType(const WidgetType& t_widgetType) { m_widgetType = t_widgetType; }
		void setIndexes(const int& t_patientIndex, const int& t_studyIndex, const int& t_seriesIndex,
		                const int& t_imageIndex);
	protected:
		QWidget* m_tabWidget = {};
		core::Series* m_series = {};
		core::Image* m_image = {};
		vtkSmartPointer<vtkRenderWindow> m_renderWindow[3] = {};
		std::unique_ptr<vtkEventFilter> m_vtkEvents = {};
		std::unique_ptr<LoadingAnimation> m_loadingAnimation = {};
		bool m_isImageLoaded = false;
		WidgetType m_widgetType = WidgetType::none;
		int m_patientIndex = {};
		int m_studyIndex = {};
		int m_seriesIndex = {};
		int m_imageIndex = {};

		virtual void initView() = 0;
		virtual void initData() = 0;
		virtual void createConnections() = 0;
		virtual void resetView() {};
		virtual void setSliderValues(const int& t_min, const int& t_max, const int& t_value) {};
		virtual void startLoadingAnimation() = 0;
		void stopLoadingAnimation();
		void focusInEvent(QFocusEvent* event) override;
	};
}
