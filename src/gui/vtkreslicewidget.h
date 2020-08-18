#pragma once

#include <vtkAbstractWidget.h>
#include <vtkSmartPointer.h>
#include <vtkImageResliceToColors.h>
#include <vtkRenderWindow.h>
#include "vtkresliceplanecursorwidget.h"
#include <vtkImageActor.h>

namespace asclepios::gui
{
	class vtkResliceCallback;

	class vtkResliceWidget final : public vtkAbstractWidget
	{
	public:
		static vtkResliceWidget* New();
		vtkTypeMacro(vtkResliceWidget, vtkAbstractWidget);
		vtkResliceWidget() = default;
		~vtkResliceWidget() = default;

		//getters
		[[nodiscard]] vtkRenderWindow* getTestRenderWindow() const { return m_testRenderWindow; }
		[[nodiscard]] int getIsCameraCentered() const { return m_isCameraCentered; }

		//setters
		void SetEnabled(int) override;
		void setImageReslicers(
			const vtkSmartPointer<vtkImageResliceToColors>& m_firstReslice,
			const vtkSmartPointer<vtkImageResliceToColors>& m_secondReslice,
			const vtkSmartPointer<vtkImageResliceToColors>& m_thirdReslice);
		vtkSmartPointer<vtkImageResliceToColors>* getImageReslicers() { return m_imageReslice; }
		void setRenderWindows(vtkSmartPointer<vtkRenderWindow>* t_windows);
		void refreshWindows(int t_windowNumber);
		void setVisible(bool);
		void setCameraCentered(int t_centered);
		void setHighQuality(int t_highQuality, int t_plane);

		void CreateDefaultRepresentation() override;
		void centerImageActors(int t_excludedCursor);
		void resetResliceCursor();

		enum widgetState { start = 0, rotate, translate };

	private:
		vtkSmartPointer<vtkImageResliceToColors> m_imageReslice[3] = {};
		vtkSmartPointer<vtkRenderWindow> m_windows[3] = {};
		vtkSmartPointer<vtkReslicePlaneCursorWidget> m_cursorWidget[3] = {};
		vtkSmartPointer<vtkResliceCallback> m_cbk[3] = {};
		vtkRenderWindow* m_testRenderWindow = nullptr;
		int m_isCameraCentered = 0;

		void resetCamera(vtkRenderWindow* t_window);
		double* getImageActorCenterPosition(vtkRenderWindow* t_window);
		void setQualityToHigh(int t_windowNumber, vtkImageActor* t_actor);
		void setQualityToLow(int t_windowNumber, vtkImageActor* t_actor);

	protected:
		friend class vtkResliceCallback;
	};
}
