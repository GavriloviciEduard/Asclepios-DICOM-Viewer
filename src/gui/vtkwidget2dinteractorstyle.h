#pragma once

#include <vtkInteractorStyleImage.h>
#include "vtkwidget2d.h"

namespace asclepios::gui
{
	class vtkWidget2DInteractorStyle final : public vtkInteractorStyleImage
	{
	public:
		static vtkWidget2DInteractorStyle* New();
		vtkTypeMacro(vtkWidget2DInteractorStyle, vtkInteractorStyleImage);

		vtkWidget2DInteractorStyle() = default;
		~vtkWidget2DInteractorStyle() = default;

		//setters
		void setWidget(vtkWidget2D* t_widget) { m_widget2D = t_widget; }
		void setSeries(core::Series* t_series) { m_series = t_series; }
		void setImage(core::Image* t_image) { m_image = t_image; }

		void changeImage(int& t_index);
		void updateOvelayImageNumber(const int& t_index) const;

	protected:
		void OnMouseMove() override;
		void OnMouseWheelForward() override;
		void OnMouseWheelBackward() override;
		void OnLeftButtonDown() override;
		void OnLeftButtonUp() override;
		void OnRightButtonDown() override;
		void OnRightButtonUp() override;
		void OnMiddleButtonDown() override;
		void OnMiddleButtonUp() override;
		void Dolly() override;

	private:
		vtkWidget2D* m_widget2D = {};
		core::Series* m_series = {};
		core::Image* m_image = {};

		void refreshImage() const;
		void updateOverlayWindowLevelApply() const;
		void updateOverlayHUValue() const;
		[[nodiscard]] core::Image* getNextImage() const;
		[[nodiscard]] core::Image* getPreviousImage() const;
	};
}
