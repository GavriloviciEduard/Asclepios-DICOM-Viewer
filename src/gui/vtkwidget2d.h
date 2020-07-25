#pragma once

#include "vtkwidgetbase.h"
#include "vtkwidgetdicom.h"
#include "vtkwidgetoverlay.h"

enum class transformationType;

namespace asclepios::gui
{
	class vtkWidget2D : public vtkWidgetBase
	{
	public:
		vtkWidget2D();
		~vtkWidget2D() = default;

		void applyTransformation(const transformationType& t_type) const;

	private:
		vtkSmartPointer<vtkDICOMReader> m_dcmReader = {};
		vtkSmartPointer<vtkWidgetDICOM> m_dcmWidget = {};
		vtkSmartPointer<vtkRenderWindowInteractor> m_interactor = {};
		vtkSmartPointer<vtkRenderer> m_vtkWidgetOverlayRenderer = {};
		std::unique_ptr<vtkWidgetOverlay> m_widgetOverlay = {};
		bool m_colorsInverted = false;

		void initRenderingLayers();
		void refreshOverlayInCorner(const int& t_corner);
		void createvtkWidgetOverlay();
	};
}
