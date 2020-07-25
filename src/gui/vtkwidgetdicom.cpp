#include "vtkwidgetdicom.h"
#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkDICOMApplyPalette.h>
#include <vtkDICOMDictHash.h>
#include <vtkDICOMMetaData.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageStack.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkWindowLevelLookupTable.h>

vtkStandardNewMacro(asclepios::gui::vtkWidgetDICOM)


void asclepios::gui::vtkWidgetDICOM::setImageReader(vtkDICOMReader* t_reader)
{
	m_reader = t_reader;
	SetInputData(m_reader->GetOutput());
	if (m_reader->HasOverlay())
	{
		createOverlayActor();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::setInvertColors(const bool& t_flag)
{
	m_windowLevelFilter->setAreColorsInverted(t_flag);
	changeWindowWidthCenter(0, 0);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::setWindowWidthCenter(const int& t_width, const int& t_center)
{
	m_windowLevelFilter->setWindowWidthCenter(t_width, t_center);
	m_windowWidth = t_width;
	m_windowCenter = t_center;
	Superclass::Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::setInitialWindowWidthCenter()
{
	if (m_imageMetaData)
	{
		if (m_imageMetaData->Get(
			DC::PhotometricInterpretation).Matches("MONOCHROME1"))
		{
			setMONOCHROME1WindowWidthCenter();
		}
		else if (m_imageMetaData->Get(
			DC::PhotometricInterpretation).Matches("MONOCHROME2"))
		{
			setMONOCHROME2WindowWidthCenter();
		}
		else if (m_imageMetaData->Get(
			DC::PhotometricInterpretation).Matches("PALETTE COLOR"))
		{
			setPALETTECOLORWindowWidthCenter();
		}
		else
		{
			GetWindowLevel()->SetOutputFormatToRGB();
		}
		GetRenderer()->GetActiveCamera()->ParallelProjectionOn();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::SetInputData(vtkImageData* in)
{
	WindowLevel->SetInputData(in);
	UpdateDisplayExtent();
}

//-----------------------------------------------------------------------------
std::tuple<int, int> asclepios::gui::vtkWidgetDICOM::getImageActorDisplayValue()
{
	auto* const actorBounds =
		GetImageActor()->GetBounds();
	vtkNew<vtkCoordinate> coordinate;
	coordinate->SetCoordinateSystemToWorld();
	coordinate->SetValue(actorBounds[0], 0, 0);
	int x = coordinate->GetComputedDisplayValue(
		GetRenderWindow()->GetRenderers()->GetFirstRenderer())[0];
	coordinate->SetValue(actorBounds[1], 0, 0);
	int y = coordinate->GetComputedDisplayValue(
		GetRenderWindow()->GetRenderers()->GetFirstRenderer())[0];
	return std::make_tuple(x, y);
}


//-----------------------------------------------------------------------------
double asclepios::gui::vtkWidgetDICOM::getZoomFactor()
{
	auto* const actorExtend =
		GetImageActor()->GetDisplayExtent();
	const auto [x, y] = getImageActorDisplayValue();
	return static_cast<double>(std::abs(y - x)) /
		static_cast<double>(static_cast<double>(actorExtend[1])
			- static_cast<double>(actorExtend[0]) + 1);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::changeWindowWidthCenter(const int& t_width, const int& t_center)
{
	if (m_isOverlay)
	{
		return;
	}
	if (auto* const lookupTable
		= GetWindowLevel()->GetLookupTable(); lookupTable)
	{
		m_windowWidth =
			dynamic_cast<vtkWindowLevelLookupTable*>(lookupTable)->GetWindow();
		m_windowCenter =
			dynamic_cast<vtkWindowLevelLookupTable*>(lookupTable)->GetLevel();
	}
	else
	{
		m_windowWidth = GetWindowLevel()->GetWindow();
		m_windowCenter = GetWindowLevel()->GetLevel();
	}
	m_windowWidth += t_width;
	m_windowCenter += t_center;
	setWindowWidthCenter(m_windowWidth, m_windowCenter);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::createOverlayActor()
{
	if (!m_overlayActor)
	{
		m_overlayActor = vtkSmartPointer<vtkImageActor>::New();
	}
	const auto mapColors = createMapColors();
	auto* const overlayImage = mapColors->GetOutput();
	overlayImage->SetExtent(m_reader->GetOutput()->GetExtent());
	overlayImage->SetSpacing(m_reader->GetOutput()->GetSpacing());
	m_overlayActor->GetMapper()->SetInputData(overlayImage);
	m_overlayActor->SetOpacity(1);
	vtkNew<vtkImageStack> stack;
	stack->AddImage(m_overlayActor);
	Renderer->AddViewProp(stack);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::setMONOCHROME1WindowWidthCenter()
{
	vtkNew<vtkWindowLevelLookupTable> table;
	table->SetWindow(std::abs(m_windowWidth));
	table->SetLevel(m_windowCenter);
	table->SetInverseVideo(true);
	table->Build();
	table->BuildSpecialColors();
	GetWindowLevel()->SetLookupTable(table);
	m_windowLevelFilter->setDICOMWidget(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::setMONOCHROME2WindowWidthCenter()
{
	m_windowLevelFilter->setDICOMWidget(this);
	m_windowLevelFilter->setWindowWidthCenter(m_windowWidth, m_windowCenter);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetDICOM::setPALETTECOLORWindowWidthCenter()
{
	auto* const input = GetInputAlgorithm()->GetOutputPort();
	vtkNew<vtkDICOMApplyPalette> palette;
	palette->SetInputConnection(input);
	palette->Update();
	SetInputData(palette->GetOutput());
	GetWindowLevel()->SetOutputFormatToRGB();
}

//-----------------------------------------------------------------------------
vtkSmartPointer<vtkImageMapToWindowLevelColors> asclepios::gui::vtkWidgetDICOM::createMapColors() const
{
	auto mapColors = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
	mapColors->SetInputData(m_reader->GetOutput(1));
	mapColors->SetLevel(0);
	mapColors->SetWindow(0);
	mapColors->Update();
	return mapColors;
}
