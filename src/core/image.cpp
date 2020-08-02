#include "image.h"

vtkSmartPointer<vtkDICOMReader> asclepios::core::Image::getImageReader() const
{
	if (m_imageReader)
	{
		return vtkSmartPointer<vtkDICOMReader>(m_imageReader);
	}
	vtkNew<vtkDICOMReader> m_imageReader;
	m_imageReader->SetFileName(m_path.c_str());
	m_imageReader->Update();
	return m_imageReader;
}

//-----------------------------------------------------------------------------
bool asclepios::core::Image::equal(Image* t_image) const
{
	return getParentObject() == t_image->getParentObject() &&
		getImagePath() == t_image->getImagePath() &&
		getSOPInstanceUID() == t_image->getSOPInstanceUID() &&
		getClassUID() == t_image->getClassUID() &&
		getFrameOfRefernceID() == t_image->getFrameOfRefernceID() &&
		getModality() == t_image->getModality() &&
		getWindowCenter() == t_image->getWindowCenter() &&
		getWindowWidth() == t_image->getWindowWidth() &&
		getRows() == t_image->getRows() &&
		getColumns() == t_image->getColumns() &&
		getNumberOfFrames() == t_image->getNumberOfFrames() &&
		getSliceLocation() == t_image->getSliceLocation() &&
		getAcquisitionNumber() == t_image->getAcquisitionNumber() &&
		getIsMultiFrame() == t_image->getIsMultiFrame();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Image::isLess(Image* t_lhs, Image* t_rhs)
{
	if (t_lhs->getSliceLocation() == t_rhs->getSliceLocation())
	{
		return t_lhs->getAcquisitionNumber() < t_rhs->getAcquisitionNumber();
	}
	return t_lhs->getSliceLocation() < t_rhs->getSliceLocation();
}
