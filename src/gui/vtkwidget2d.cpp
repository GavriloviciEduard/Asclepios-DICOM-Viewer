#include "vtkwidget2d.h"
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include "utils.h"

asclepios::gui::vtkWidget2D::vtkWidget2D()
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2D::applyTransformation(const transformationType& t_type) const
{
	if (!m_activeRenderWindow)
	{
		return;
	}
	auto* const renderer =
		m_activeRenderWindow->GetRenderers()->GetFirstRenderer();
	if (!renderer)
	{
		return;
	}
	auto* const camera = renderer->GetActiveCamera();
	auto* const cameraPosition = camera->GetPosition();
	try
	{
		switch (t_type)
		{
		case transformationType::flipHorizontal:
			camera->SetPosition(cameraPosition[0],
			                    cameraPosition[1], -cameraPosition[2]);
			break;
		case transformationType::flipVertical:
			camera->SetPosition(cameraPosition[0],
			                    cameraPosition[1], -cameraPosition[2]);
			camera->Roll(180);
			break;
		case transformationType::rotateLeft:
			camera->Roll(90);
			break;
		case transformationType::rotateRight:
			camera->Roll(-90);
			break;
		case transformationType::invert:
			//todo
			break;
		default:
			break;
		}
	}
	catch (const std::exception& ex)
	{
		//todo log
	}
	m_activeRenderWindow->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2D::initRenderingLayers()
{
	if (m_renderWindows[0])
	{
		auto* const renderer = m_renderWindows[0]->GetRenderers()->GetFirstRenderer();
		renderer->SetLayer(0);
		renderer->SetInteractive(1);
		m_vtkWidgetOverlayRenderer = vtkSmartPointer<vtkRenderer>::New();
		m_vtkWidgetOverlayRenderer->SetLayer(1);
		m_vtkWidgetOverlayRenderer->SetInteractive(0);
		m_renderWindows[0]->SetNumberOfLayers(2);
		m_renderWindows[0]->AddRenderer(m_vtkWidgetOverlayRenderer);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2D::refreshOverlayInCorner(const int& t_corner)
{
	m_widgetOverlay->setOverlayInCorner(t_corner);
	m_renderWindows[0]->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget2D::createvtkWidgetOverlay()
{
	if (!m_widgetOverlay)
	{
		m_widgetOverlay = std::make_unique<vtkWidgetOverlay>();
	}
	m_widgetOverlay->setRenderer(m_vtkWidgetOverlayRenderer);
	m_widgetOverlay->createOverlay(m_renderWindows[0],
		m_dcmReader->GetMetaData());
}
