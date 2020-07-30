#include "vtkwidget2dinteractorstyle.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkObjectFactory.h>
#include "utils.h"


vtkStandardNewMacro(asclepios::gui::vtkWidget2DInteractorStyle);

void asclepios::gui::vtkWidget2DInteractorStyle::OnMouseMove()
{
	switch (State)
	{
	case VTKIS_WINDOW_LEVEL:
		updateOverlayWindowLevelApply();
		return;
	default:
		updateOverlayHUValue();
		Interactor->Render();
		break;
	}
	vtkInteractorStyleImage::OnMouseMove();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnMouseWheelForward()
{
	try
	{
		if (m_image)
		{
			if (!m_image->getIsMultiFrame())
			{
				auto* const previousImage = m_series->getPreviousSingleFrameImage(m_image);
				if (!m_image->equal(previousImage))
				{
					int index = m_series->
						findImageIndex(m_series->getSinlgeFrameImages(), m_image);
					changeImage(--index);
				}
			}
			else
			{
				auto indexImage = m_widget2D->getDCMWidget()->GetSlice();
				if (indexImage != m_widget2D->getDCMWidget()->GetSliceMin())
				{
					changeImage(--indexImage);
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		//todo log
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnMouseWheelBackward()
{
	try
	{
		if (m_image)
		{
			if (!m_image->getIsMultiFrame())
			{
				auto* const nextImage = m_series->getNextSingleFrameImage(m_image);
				if (!m_image->equal(nextImage))
				{
					int index = m_series->
						findImageIndex(m_series->getSinlgeFrameImages(), m_image);
					changeImage(++index);
				}
			}
			else
			{
				auto indexImage = m_widget2D->getDCMWidget()->GetSlice();
				if (indexImage != m_widget2D->getDCMWidget()->GetSliceMax())
				{
					changeImage(++indexImage);
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		//todo log
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::changeImage(int& t_index)
{
	if (m_image)
	{
		if (!m_image->getIsMultiFrame())
		{
			m_image = m_series->getSingleFrameImageByIndex(t_index);
			refreshImage();
			Interactor->InvokeEvent(imageChanged);
			if (m_image->getModality() == "MR")
			{
				m_widget2D->updateOverlayWindowLevelApply(m_image->getWindowWidth(),
				                                          m_image->getWindowCenter(), false);;
			}
		}
		else
		{
			m_widget2D->getDCMWidget()->SetSlice(t_index);
			Interactor->InvokeEvent(imageChanged, &t_index);
		}
		updateOvelayImageNumber(t_index);
	}
	else
	{
		throw std::runtime_error("Image is null!");
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnLeftButtonDown()
{
	StartWindowLevel();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnLeftButtonUp()
{
	StopState();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnRightButtonDown()
{
	StartDolly();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnRightButtonUp()
{
	StopState();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnMiddleButtonDown()
{
	StartPan();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::OnMiddleButtonUp()
{
	StopState();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::Dolly()
{
	vtkInteractorStyleImage::Dolly();
	m_widget2D->updateOverlayZoomFactor();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::refreshImage() const
{
	if (!m_image)
	{
		throw std::runtime_error("Image is null!");
	}
	if (m_widget2D && m_widget2D->getImageReader())
	{
		m_widget2D->setImage(m_image);
		m_widget2D->setImageReader(m_image->getImageReader());
		m_widget2D->getDCMWidget()->setImageReader(m_image->getImageReader());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::updateOverlayWindowLevelApply() const
{
	const auto x =
		Interactor->GetEventPosition()[0] - Interactor->GetLastEventPosition()[0];
	const auto y =
		Interactor->GetEventPosition()[1] - Interactor->GetLastEventPosition()[1];
	m_widget2D->updateOverlayWindowLevelApply(x, y, true);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::updateOverlayHUValue() const
{
	const auto x = Interactor->GetEventPosition()[0];
	const auto y = Interactor->GetEventPosition()[1];
	double world[4];
	ComputeDisplayToWorld(m_widget2D->getDCMWidget()->GetRenderWindow()
	                                ->GetRenderers()->GetFirstRenderer(), x, y, 0, world);
	m_widget2D->updateOverlayHUValue(world[0] / m_image->getPixelSpacingX(),
		world[1] / m_image->getPixelSpacingY());
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2DInteractorStyle::updateOvelayImageNumber(const int& t_index) const
{
	const auto max = !m_image->getIsMultiFrame()
		                 ? m_series->getSinlgeFrameImages().size()
		                 : m_image->getNumberOfFrames();
	m_widget2D->updateOvelayImageNumber(t_index, max,
	                                    std::stoi(m_series->getNumber()));
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::gui::vtkWidget2DInteractorStyle::getNextImage() const
{
	return m_image ? m_series->getNextSingleFrameImage(m_image) : nullptr;
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::gui::vtkWidget2DInteractorStyle::getPreviousImage() const
{
	return m_image ? m_series->getPreviousSingleFrameImage(m_image) : nullptr;
}
