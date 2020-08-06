#pragma once
#include <vtkImageViewer2.h>

namespace asclepios::gui
{
	class WindowLevelFilter
	{
	public:
		WindowLevelFilter() = default;
		~WindowLevelFilter() = default;

		//getters
		[[nodiscard]] vtkImageViewer2* getDICOMWidget() const { return m_dicomWidget; }
		[[nodiscard]] bool getAreColorosInverted() const { return m_invert; }

		//setters
		void setDICOMWidget(vtkImageViewer2* t_widget) { m_dicomWidget = t_widget; }
		void setAreColorsInverted(const bool& t_flag) { m_invert = t_flag; }
		void setWindowWidthCenter(const int& t_width, const int& t_center) const;

	private:
		vtkImageViewer2* m_dicomWidget = {};
		bool m_invert = false;
	};
}
