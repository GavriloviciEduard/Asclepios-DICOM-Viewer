#include "windowlevelfilter.h"
#include <vtkWindowLevelLookupTable.h>
#include <vtkImageMapToWindowLevelColors.h>

void asclepios::gui::WindowLevelFilter::setWindowWidthCenter(const int& t_width, const int& t_center) const
{
	if (!m_dicomWidget)
	{
		return;
	}
	if (auto* const lookupTable =
		m_dicomWidget->GetWindowLevel()->GetLookupTable(); lookupTable)
	{
		auto* const table = dynamic_cast<vtkWindowLevelLookupTable*>(lookupTable);
		table->SetInverseVideo(!m_invert);
		table->SetWindow(t_width);
		table->SetLevel(t_center);
	}
	else
	{
		m_dicomWidget->SetColorLevel(t_center);
		int level = t_width;
		int sign = 1;
		if (level < 0)
		{
			level *= -1;
		}
		if (m_invert)
		{
			sign *= -1;
		}
		m_dicomWidget->SetColorWindow(sign * level);
	}
}
