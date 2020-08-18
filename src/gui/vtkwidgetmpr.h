#pragma once

#include <vtkAxisActor2D.h>
#include "vtkwidgetbase.h"
#include "mprmaker.h"
#include "vtkwidgetoverlay.h"
#include "vtkreslicewidget.h"
#include "vtkwidgetmprcallback.h"

namespace asclepios::gui
{
	class vtkWidgetMPR final : public vtkWidgetBase
	{
	public:
		vtkWidgetMPR() { initializeWidget(); }
		~vtkWidgetMPR();

		//getters
		[[nodiscard]] int getNumberOfRenderWindow(vtkRenderWindow* t_window) const;
		

		//setters
		void setInteractor([[maybe_unused]] const vtkSmartPointer<vtkRenderWindowInteractor>& t_interactor) override {}
		void setActiveRenderWindow(vtkRenderWindow* t_window) { m_activeRenderWindow = t_window; }
		void setWindowLevel(const int& t_window, const int& t_level);
		void setCameraCentered(const int& t_centered) const;
		void setShowCursor(const bool& t_flag) const { m_resliceWidget->setVisible(t_flag); }
		
		void render() override;
		void resetResliceWidget();
		void changeWindowLevel(vtkRenderWindow* t_renderWindow, const int& t_window,
			const int& t_level, bool t_append = true);
		void create3DMatrix() const;

	private:
		vtkSmartPointer<vtkWidgetMPRCallback> m_callback = {};
		vtkSmartPointer<vtkResliceWidget> m_resliceWidget = {};
		std::unique_ptr<MPRMaker> m_mprMaker = {};
		std::unique_ptr<vtkWidgetOverlay> m_widgetOverlay[3] = {};
		unsigned int m_callbackTags[3] = {};

		void initializeWidget();
		void createResliceWidget();
		void createVTKkWidgetOverlay(vtkRenderWindow* t_window, int& t_windowNumber);
		void refreshOverlayInCorner(vtkRenderWindow* t_window, int t_windowNumber, int t_corner);
		[[nodiscard]] vtkAxisActor2D* createScaleActor(vtkRenderWindow* t_window);
	};
}
