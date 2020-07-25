#pragma once

#include <vtkDICOMReader.h>
#include <vtkImageViewer2.h>
#include <vtkSmartPointer.h>
#include <tuple>
#include "windowlevelfilter.h"

namespace asclepios::gui
{
	class vtkWidgetDICOM final : public vtkImageViewer2
	{
	public:
		static vtkWidgetDICOM* New();
		vtkTypeMacro(vtkWidgetDICOM, vtkImageViewer2);

		vtkWidgetDICOM() = default;
		~vtkWidgetDICOM() = default;

		//getters
		[[nodiscard]] vtkDICOMMetaData* getImageMetaData() const { return m_imageMetaData; }
		[[nodiscard]] int getWindowWidth() const { return m_windowWidth; }
		[[nodiscard]] int getWindowCenter() const { return m_windowCenter; }
		[[nodiscard]] bool getIsOverlay() const { return m_isOverlay; }
		[[nodiscard]] double getZoomFactor();

		//setters
		void setImageMetaData(vtkDICOMMetaData* t_metaData) { m_imageMetaData = t_metaData; }
		void setWindowWidth(const int& t_windowWidth) { m_windowWidth = t_windowWidth; }
		void setWindowCenter(const int& t_windowCenter) { m_windowCenter = t_windowCenter; }
		void setIsOverlay(const bool& t_flag) { m_isOverlay = t_flag; }
		void setImageReader(vtkDICOMReader* t_reader);
		void setInvertColors(const bool& t_flag);
		void setWindowWidthCenter(const int& t_width, const int& t_center);
		void setInitialWindowWidthCenter();
		void SetInputData(vtkImageData* in) override;

		void changeWindowWidthCenter(const int& t_width, const int& t_center);


	private:
		vtkDICOMMetaData* m_imageMetaData = {};
		vtkDICOMReader* m_reader = {};
		vtkSmartPointer<vtkImageActor> m_overlayActor = {};
		int m_windowWidth = {};
		int m_windowCenter = {};
		std::unique_ptr<WindowLevelFilter> m_windowLevelFilter = {};
		bool m_isOverlay = false;

		void createOverlayActor();
		void setMONOCHROME1WindowWidthCenter();
		void setMONOCHROME2WindowWidthCenter();
		void setPALETTECOLORWindowWidthCenter();
		[[nodiscard]] vtkSmartPointer<vtkImageMapToWindowLevelColors> createMapColors() const;
		[[nodiscard]] std::tuple<int, int> getImageActorDisplayValue();
	};
}
