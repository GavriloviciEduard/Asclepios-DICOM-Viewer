#include "vtkwidgetmpr.h"
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include "image.h"
#include "vtkwidgetmprinteractorstyle.h"
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkDICOMMetaData.h>
#include <vtkScalarsToColors.h>


asclepios::gui::vtkWidgetMPR::~vtkWidgetMPR()
{
	for (int i = 0; i < 3; ++i)
	{
		m_renderWindows[i]->RemoveObserver(m_callbackTags[i]);
	}
}

//-----------------------------------------------------------------------------
int asclepios::gui::vtkWidgetMPR::getNumberOfRenderWindow(vtkRenderWindow* t_window) const
{
	for (auto i = 0; i < 3; ++i)
	{
		if (m_renderWindows[i] == t_window)
		{
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::setWindowLevel(const int& t_window, const int& t_level)
{
	for (const auto& window : m_renderWindows)
	{
		changeWindowLevel(window, t_window, t_level, false);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::setCameraCentered(const int& t_centered) const
{
	if (m_resliceWidget)
	{
		m_resliceWidget->setCameraCentered(t_centered);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::render()
{
	if (!m_mprMaker)
	{
		return;
	}
	m_mprMaker->SetRenderWindows(
		m_renderWindows[0],
		m_renderWindows[1],
		m_renderWindows[2]);
	m_mprMaker->setInitialWindow(m_image->getWindowCenter());
	m_mprMaker->setInitialLevel(m_image->getWindowWidth());
	m_mprMaker->createMPR();
	m_callback = vtkSmartPointer<vtkWidgetMPRCallback>::New();
	m_callback->setWidget(this);
	for (auto i = 0; i < 3; ++i)
	{
		vtkNew<vtkWidgetMPRInteractorStyle> interactorStyle;
		interactorStyle->setImageReslice(m_mprMaker->getImageReslice(i));
		const int minExtendPosition = i * 2;
		const int maxExtendPosition = i * 2 + 1;
		interactorStyle->setMinMax(
			m_mprMaker->getInputData()->GetExtent()[minExtendPosition],
			m_mprMaker->getInputData()->GetExtent()[maxExtendPosition]);
		interactorStyle->SetCurrentImageNumber(m_mprMaker->getCenterSliceZPosition(i));
		interactorStyle->setWidget(this);
		m_renderWindows[i]->GetInteractor()->SetInteractorStyle(interactorStyle);
		interactorStyle->SetCurrentRenderer(m_renderWindows[i]->GetRenderers()->GetFirstRenderer());
		interactorStyle->setAxisActor2D(
			createScaleActor(m_renderWindows[i]));
		interactorStyle->rescaleAxisActor();
		m_widgetOverlay[i] = std::make_unique<vtkWidgetOverlay>();
		createVTKkWidgetOverlay(m_renderWindows[i], i);
		m_callbackTags[i] = m_renderWindows[i]->AddObserver(vtkCommand::ModifiedEvent,
		                                                    m_callback);
	}
	createResliceWidget();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::resetResliceWidget()
{
	if (m_resliceWidget)
	{
		m_mprMaker->resetMatrixesToInitialPosition();
		m_mprMaker->resetWindowLevel();
		m_resliceWidget->centerImageActors(-1);
		m_resliceWidget->resetResliceCursor();
		m_renderWindows[0]->Render();
		m_renderWindows[1]->Render();
		m_renderWindows[2]->Render();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::changeWindowLevel(vtkRenderWindow* t_renderWindow, const int& t_window,
                                                     const int& t_level, const bool t_append)
{
	int window;
	int level;
	if (t_append)
	{
		auto* const range =
			m_mprMaker->getColorMapScalar()->GetRange();
		window = range[1] - range[0];
		level = range[0] + window / 2;
		window += t_window;
		level += t_level;
	}
	else
	{
		window = t_window;
		level = t_level;
	}
	m_mprMaker->getColorMapScalar()->SetRange(level - 0.5 * window, level + 0.5 * window);
	auto const windowNumber =
		getNumberOfRenderWindow(t_renderWindow);
	if (windowNumber == -1)
	{
		return;
	}
	std::string ww;
	ww.append("WW: ").append(std::to_string(window));
	std::string wl;
	wl.append(" / WL: ").append(std::to_string(level)).append("\n");
	m_widgetOverlay[windowNumber]->updateOverlayInCorner(2, "2625616", ww);
	m_widgetOverlay[windowNumber]->updateOverlayInCorner(2, "2625617", wl);
	refreshOverlayInCorner(t_renderWindow, windowNumber, 2);
}

void asclepios::gui::vtkWidgetMPR::create3DMatrix() const
{
	if (m_image)
	{
		m_mprMaker->setImage(m_image);
	}
	else
	{
		m_mprMaker->setSeries(m_series);
		m_mprMaker->setImage(m_series->getSinlgeFrameImages().begin()->get());
	}
	m_mprMaker->create3DMatrix();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::initializeWidget()
{
	m_mprMaker = std::make_unique<MPRMaker>();
	for (auto& window : m_renderWindows)
	{
		if (!window)
		{
			window = vtkSmartPointer<vtkRenderWindow>::New();
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::createResliceWidget()
{
	if (!m_resliceWidget)
	{
		m_resliceWidget =
			vtkSmartPointer<vtkResliceWidget>::New();
	}
	m_resliceWidget->setRenderWindows(m_renderWindows);
	m_resliceWidget->setImageReslicers(
		m_mprMaker->getImageReslice(0),
		m_mprMaker->getImageReslice(1),
		m_mprMaker->getImageReslice(2));
	m_resliceWidget->SetInteractor(m_renderWindows[0]->GetInteractor());
	m_resliceWidget->SetEnabled(1);
	m_resliceWidget->setVisible(false);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::createVTKkWidgetOverlay(vtkRenderWindow* t_window, int& t_windowNumber)
{
	auto* const renderer =
		t_window->GetRenderers()->GetFirstRenderer();
	auto* const metadata =
		m_image->getIsMultiFrame()
			? m_image->getImageReader()->GetMetaData()
			: m_series->getMetaDataForSeries().Get();
	m_widgetOverlay[t_windowNumber]->setRenderer(renderer);
	m_widgetOverlay[t_windowNumber]->createOverlay(t_window, metadata);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetMPR::refreshOverlayInCorner(vtkRenderWindow* t_window, const int t_windowNumber,
                                                          const int t_corner)
{
	m_widgetOverlay[t_windowNumber]->setOverlayInCorner(t_corner);
	t_window->Render();
}

//-----------------------------------------------------------------------------
vtkAxisActor2D* asclepios::gui::vtkWidgetMPR::createScaleActor(vtkRenderWindow* t_window)
{
	vtkNew<vtkAxisActor2D> m_scaleActor;
	m_scaleActor->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
	m_scaleActor->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
	m_scaleActor->SetNumberOfLabels(5);
	m_scaleActor->LabelVisibilityOff();
	m_scaleActor->AdjustLabelsOff();
	m_scaleActor->GetTitleTextProperty()->SetBold(1);
	m_scaleActor->GetTitleTextProperty()->SetItalic(1);
	m_scaleActor->GetTitleTextProperty()->SetShadow(1);
	m_scaleActor->GetTitleTextProperty()->SetFontFamilyToArial();
	m_scaleActor->GetProperty()->SetColor(1, 1, 1);
	t_window->GetRenderers()->GetFirstRenderer()->AddActor(m_scaleActor);
	return m_scaleActor;
}
