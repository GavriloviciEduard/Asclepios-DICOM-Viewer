#include "image.h"

vtkSmartPointer<vtkDICOMReader> asclepios::core::Image::getImageReader() const
{
	//todo this is blocking the first time, must be made async
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
bool asclepios::core::Image::isLess(Image* t_lhs, Image* t_rhs)
{
	return t_lhs->getParentObject() == t_rhs->getParentObject() &&
		t_lhs->getImagePath() == t_rhs->getImagePath() &&
		t_lhs->getSOPInstanceUID() == t_rhs->getSOPInstanceUID() &&
		t_lhs->getClassUID() == t_rhs->getClassUID() &&
		t_lhs->getFrameOfRefernceID() == t_rhs->getFrameOfRefernceID() &&
		t_lhs->getModality() == t_rhs->getModality() &&
		t_lhs->getWindowCenter() == t_rhs->getWindowCenter() &&
		t_lhs->getWindowWidth() == t_rhs->getWindowWidth() &&
		t_lhs->getRows() == t_rhs->getRows() &&
		t_lhs->getColumns() == t_rhs->getColumns() &&
		t_lhs->getNumberOfFrames() == t_rhs->getNumberOfFrames() &&
		t_lhs->getSliceLocation() == t_rhs->getSliceLocation() &&
		t_lhs->getIsMultiFrame() == t_rhs->getIsMultiFrame();
}
