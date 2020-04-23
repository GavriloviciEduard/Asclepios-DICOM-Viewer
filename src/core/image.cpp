#include "image.h"

bool asclepios::core::Image::operator==(const Image& t_rhs) const
{
	return getParentObject() == t_rhs.getParentObject() &&
		getImagePath() == t_rhs.getImagePath() &&
		getSOPInstanceUID() == t_rhs.getSOPInstanceUID() &&
		getClassUID() == t_rhs.getClassUID() &&
		getFrameOfRefernceID() == t_rhs.getFrameOfRefernceID() &&
		getModality() == t_rhs.getModality() &&
		getWindowCenter() == t_rhs.getWindowCenter() &&
		getWindowWidth() == t_rhs.getWindowWidth() &&
		getRows() == t_rhs.getRows() &&
		getColumns() == t_rhs.getColumns() &&
		getNumberOfFrames() == t_rhs.getNumberOfFrames() &&
		getSliceLocation() == t_rhs.getSliceLocation() &&
		getIsMultiFrame() == t_rhs.getIsMultiFrame();
}
