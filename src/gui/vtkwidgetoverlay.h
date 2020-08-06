#pragma once

#include <memory>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include "corneroverlay.h"
#include "overlayinfo.h"

class vtkDICOMMetaData;

namespace asclepios::gui
{
	class vtkWidgetOverlay
	{
	public:
		vtkWidgetOverlay();
		~vtkWidgetOverlay();

		//getters
		[[nodiscard]] vtkRenderer* getRenderer() const { return m_render; }
		[[nodiscard]] vtkOpenGLTextActor* getOverlayInCorner(const int& t_corner)
		{ return m_cornersOfOverlay[t_corner]->getTextActor(); }

		//setters
		void setRenderer(vtkRenderer* t_renderer) { m_render = t_renderer; }
		void setOverlayInCorner(const int& t_corner) { m_cornersOfOverlay[t_corner]->initTextActor(); }

		void createOverlay(vtkRenderWindow* t_renderWindow, vtkDICOMMetaData* t_metadata);
		void clearOverlay();
		void updateOverlayInCorner(const int& t_corner, const std::string& t_key,const std::string& t_value)
		{ m_cornersOfOverlay[t_corner]->setOverlayInfo(t_key, t_value); }
		void positionOverlay();

	private:
		vtkSmartPointer<vtkTextProperty> m_textProperty[4] = {};
		std::unique_ptr<CornerOverlay> m_cornersOfOverlay[4] = {};
		vtkRenderer* m_render = {};
		std::vector<std::unique_ptr<OverlayInfo>> m_overlays;
		unsigned long m_observerTag = 0;
		

		void createOverlayInCorners();
		void readOverlayInfo();
		void createOverlayInfo(vtkDICOMMetaData* t_metadata);
		void setPositionOfOverlayInCorner(const int& x, const int& y, const int& nr);
		void createCallback(vtkRenderWindow* t_renderWindow);
		[[nodiscard]] std::string replaceInvalidCharactersInString(const std::string& t_string);
		[[nodiscard]] std::tuple<int, int> computeCurrentOverlayPosition(const int& i, const int& j, const int* size);
	};
}
