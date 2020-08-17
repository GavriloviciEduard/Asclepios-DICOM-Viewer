#pragma once

#include <vtkAxisActor2D.h>
#include <vtkImageResliceToColors.h>
#include <vtkInteractorStyleImage.h>
#include "utils.h"

namespace asclepios::gui
{
	class vtkWidgetMPR;

	class vtkWidgetMPRInteractorStyle final : public vtkInteractorStyleImage
	{
	public:
		static vtkWidgetMPRInteractorStyle* New();
		vtkTypeMacro(vtkWidgetMPRInteractorStyle, vtkInteractorStyleImage);
		vtkWidgetMPRInteractorStyle() = default;
		~vtkWidgetMPRInteractorStyle() = default;

		//getters
		[[nodiscard]] int getMax() const { return m_max; }
		[[nodiscard]] int getMin() const { return m_min; }
		[[nodiscard]] vtkWidgetMPR* getWidget() const { return m_widget; }
		[[nodiscard]] vtkAxisActor2D* getAxisActor2D() const { return m_axisActor; }
		[[nodiscard]] vtkImageResliceToColors* getImageReslice() const { return m_imageReslice; }

		//setters
		void setMinMax(const int& t_min, const int& t_max) { m_min = t_min; m_max = t_max; }
		void setWidget(vtkWidgetMPR* t_widget) { m_widget = t_widget; }
		void setAxisActor2D(vtkAxisActor2D* t_actor) { m_axisActor = t_actor; }
		void setImageReslice(vtkImageResliceToColors* t_imageReslice) { m_imageReslice = t_imageReslice; }
		
		void rescaleAxisActor();
		void moveToSlice(const int& t_number);

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

	private:
		vtkWidgetMPR* m_widget = {};
		vtkAxisActor2D* m_axisActor = {};
		vtkImageResliceToColors* m_imageReslice = {};
		int m_max = 0;
		int m_min = 0;

		void startAction(const transformationType& t_action);
		void moveSlice(const int& t_delta);
	};
}
