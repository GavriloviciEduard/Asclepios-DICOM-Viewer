#pragma once
#include <series.h>
#include <vtkRenderWindow.h>

namespace asclepios::gui
{
	class vtkWidgetBase
	{
	public:
		vtkWidgetBase() = default;
		virtual ~vtkWidgetBase() = default;

		//getters
		[[nodiscard]] vtkSmartPointer<vtkRenderWindow>* getRenderWindows() { return m_renderWindows; }
		[[nodiscard]] vtkRenderWindow* getActiveRenderWindow() const { return m_activeRenderWindow; }
		[[nodiscard]] core::Image* getImage() const { return m_image; }
		[[nodiscard]] core::Series* getSeries() const { return m_series; }

		//setters
		void setImage(core::Image* t_image) { m_image = t_image; }
		void setRenderWindow(const vtkSmartPointer<vtkRenderWindow>& t_renderWindow) { m_renderWindows[0] = t_renderWindow; }
		void setRenderWindowsMPR(const vtkSmartPointer<vtkRenderWindow>& t_sagittal,
			const vtkSmartPointer<vtkRenderWindow>& t_coronal,
			const vtkSmartPointer<vtkRenderWindow>& t_axial);
		virtual void setInteractor(const vtkSmartPointer<vtkRenderWindowInteractor>& t_interactor) = 0;

		virtual void render() = 0;
		
	protected:
		vtkSmartPointer<vtkRenderWindow> m_renderWindows[3] = {};
		vtkRenderWindow* m_activeRenderWindow = nullptr;
		core::Series* m_series = {};
		core::Image* m_image = {};
	};
}
