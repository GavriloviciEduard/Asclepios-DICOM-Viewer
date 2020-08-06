#include "vtkwidget3d.h"
#include <vtkRenderer.h>
#include <vtkDICOMMetaData.h>
#include <vtkWidgetRepresentation.h>
#include <vtkVolumeProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkContourValues.h>

void asclepios::gui::vtkWidget3D::initWidget()
{
	m_renderWindows[0] = vtkSmartPointer<vtkRenderWindow>::New();
	m_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_volume = vtkSmartPointer<vtkVolume>::New();
	m_transferFunction = std::make_unique<TransferFunction>();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::initBoxWidget()
{
	if (!m_renderer || !m_volume)
	{
		return;
	}
	m_boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
	m_boxWidget->SetInteractor(m_renderer->GetRenderWindow()->GetInteractor());
	m_boxWidget->CreateDefaultRepresentation();
	m_boxWidget->GetRepresentation()->SetPlaceFactor(1);
	m_boxWidget->GetRepresentation()->PlaceWidget(m_volume->GetBounds());
	initBoxWidgetCallback();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::initBoxWidgetCallback()
{
	m_boxWidgetCallback = vtkSmartPointer<vtkBoxWidget3DCallback>::New();
	m_boxWidgetCallback->setVolume(m_volume);
	m_boxWidget->AddObserver(vtkCommand::InteractionEvent, m_boxWidgetCallback);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::initInteractorStyle()
{
	m_interactorStyle =
		vtkSmartPointer<vtkWidget3DInteractorStyle>::New();
	m_interactorStyle->setWidget(this);
	m_interactorStyle->setTransferFunction(m_transferFunction.get());
	m_interactor->SetInteractorStyle(m_interactorStyle);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::setVolumeMapperBlend() const
{
	if (m_isIsosurfaceActive)
	{
		m_mapper->SetBlendMode(vtkVolumeMapper::ISOSURFACE_BLEND);
	}
	else if (m_isMaximumIntensityProjectionActive)
	{
		m_mapper->SetBlendMode(vtkVolumeMapper::MAXIMUM_INTENSITY_BLEND);
	}
	else
	{
		m_mapper->SetBlendMode(vtkVolumeMapper::COMPOSITE_BLEND);
	}
}

//-----------------------------------------------------------------------------
std::tuple< int, int> asclepios::gui::vtkWidget3D::getWindowLevel() const
{
	const auto imageReader =
		m_image->getImageReader();
	return std::make_tuple< int, int>(imageReader->
		GetMetaData()->Get(DC::WindowCenter).AsInt(),
		imageReader->GetMetaData()->Get(DC::WindowWidth).AsInt());
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::setIsIsosurfaceActive(const bool& t_flag)
{
	m_isIsosurfaceActive = t_flag;
	m_transferFunction->setIsosurfaceFunction(-500);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::setIsMaximumIntensityProjectionActive(const bool& t_flag)
{
	m_isMaximumIntensityProjectionActive = t_flag;
	m_transferFunction->setMaximumIntensityProjectionFunction(0, 0);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::setFilter(const QString& t_filePath) const
{
	try
	{
		m_transferFunction->loadFilterFromFile(t_filePath);
		m_interactorStyle->setTransferFunction(m_transferFunction.get());
		updateFilter();
	}
	catch (const std::exception& ex)
	{
		//todo log
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::render()
{
	m_renderWindows[0]->OffScreenRenderingOn();
	setVolumeMapperBlend();
	const auto [window, level] = getWindowLevel();
	if(m_isIsosurfaceActive)
	{
		m_mapper->AutoAdjustSampleDistancesOff();
		m_mapper->SetSampleDistance(0.5);
		m_mapper->SetAverageIPScalarRange(200, 250);
	}
	else
	{
		m_transferFunction->updateWindowLevel(window, level);
	}
	m_volume->SetMapper(m_mapper);
	m_renderer->AddActor(m_volume);
	m_renderWindows[0]->AddRenderer(m_renderer);
	m_renderWindows[0]->Render();
	m_renderWindows[0]->OffScreenRenderingOff();
	auto* const extend = m_volume->GetBounds();
	m_volume->SetOrigin(extend[0] + (extend[1] - extend[0]) / 2,
		extend[2] + (extend[3] - extend[2]) / 2, 0);
	updateFilter();
	initInteractorStyle();
	initBoxWidget();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::activateBoxWidget(const bool& t_flag) const
{
	if (m_boxWidget)
	{
		m_boxWidget->SetEnabled(t_flag);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidget3D::updateFilter() const
{
	m_volume->GetProperty()->SetInterpolationTypeToLinear();
	m_volume->GetProperty()->SetScalarOpacity(m_transferFunction->getOpacityFunction());
	m_volume->GetProperty()->SetAmbient(m_transferFunction->getAmbient());
	m_volume->GetProperty()->SetColor(m_transferFunction->getColorFunction());
	m_volume->GetProperty()->SetDiffuse(m_transferFunction->getDiffuse());
	m_volume->GetProperty()->SetSpecular(m_transferFunction->getSpecular());
	m_volume->GetProperty()->SetSpecularPower(m_transferFunction->getSpecularPower());
	m_volume->GetProperty()->GetIsoSurfaceValues()->SetValue(0, m_transferFunction->getIsosurfaceValue());
	(m_transferFunction->getHasShade()) ? m_volume->GetProperty()->ShadeOn() : m_volume->GetProperty()->ShadeOff();
}
