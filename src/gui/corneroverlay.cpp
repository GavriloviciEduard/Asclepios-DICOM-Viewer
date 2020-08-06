#include "corneroverlay.h"

vtkOpenGLTextActor* asclepios::gui::CornerOverlay::getTextActor()
{
	initTextActor();
	return m_textActor;
}

//-----------------------------------------------------------------------------
std::string asclepios::gui::CornerOverlay::getOverlayFromInfo()
{
	std::string values;
	for (const auto& value : m_overlaysInfo)
	{
		values.append(value.second);
	}
	return values;
}
