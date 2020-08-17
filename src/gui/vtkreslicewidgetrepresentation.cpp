#include "vtkreslicewidgetrepresentation.h"
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkCoordinate.h>
#include <vtkEllipseArcSource.h>
#include <vtkHandleRepresentation.h>
#include <vtkObjectFactory.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

vtkStandardNewMacro(asclepios::gui::vtkResliceWidgetRepresentation);

asclepios::gui::vtkResliceWidgetRepresentation::vtkResliceWidgetRepresentation()
{
	initializeRepresentation();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::initializeRepresentation()
{
	m_handleRepresentation = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	m_handleRepresentation->SetSmoothMotion(1);
	m_handleRepresentation->SetHandleSize(0);
	m_handleRepresentation->SetTolerance(15);
	m_handleRepresentation->AllOff();
	m_handleRepresentation->TranslationModeOn();
	createCursor();
	instantiateHandleRepresentation();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::createCursor()
{
	m_cursorActor = vtkSmartPointer<vtkResliceActor>::New();
	m_cursorVisibility = true;
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::SetVisibility(const int _arg)
{
	m_cursorVisibility = static_cast<bool>(_arg);
}

//-----------------------------------------------------------------------------
int asclepios::gui::vtkResliceWidgetRepresentation::GetVisibility()
{
	return static_cast<int>(m_cursorVisibility);
}

//-----------------------------------------------------------------------------
int asclepios::gui::vtkResliceWidgetRepresentation::ComputeInteractionState(
	const int X, const int Y, int modify)
{
	if (m_centerMovementPointRepresentation->ComputeInteractionState(X, Y)
		!= vtkHandleRepresentation::Outside)
	{
		return handleCursor;
	};
	vtkNew<vtkCellPicker> picker;
	picker->SetTolerance(0.01);
	picker->InitializePickList();
	picker->PickFromListOn();
	picker->AddPickList(m_cursorActor->getActor());
	if (picker->Pick(X, Y, 0, Renderer))
	{
		return mprCursor;
	}
	return outside;
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::instantiateHandleRepresentation()
{
	if (!m_centerMovementPointRepresentation)
	{
		m_centerMovementPointRepresentation =
			m_handleRepresentation->NewInstance();
		m_centerMovementPointRepresentation->
			ShallowCopy(m_handleRepresentation);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::BuildRepresentation()
{
	if (GetMTime() > BuildTime ||
		getCenterMovementRepresentation()->GetMTime() > BuildTime ||
		Renderer && Renderer->GetVTKWindow() &&
		Renderer->GetVTKWindow()->GetMTime() > BuildTime)
	{
		double centerPos[3];
		m_centerMovementPointRepresentation->GetWorldPosition(centerPos);
		Renderer->GetVTKWindow()->GetSize();
		m_cursorActor->setCameraDistance(
			Renderer->GetActiveCamera()->GetDistance());
		vtkNew<vtkCoordinate> coord;
		coord->SetCoordinateSystemToDisplay();
		coord->SetValue(
			Renderer->GetVTKWindow()->GetSize()[0],
			Renderer->GetVTKWindow()->GetSize()[1], 0);
		double* size = coord->GetComputedWorldValue(Renderer);
		m_cursorActor->setCenterPosition(centerPos);
		m_cursorActor->setDisplaySize(size);
		coord->SetValue(0, 0);
		double* origin = coord->GetComputedWorldValue(Renderer);
		m_cursorActor->setDisplayOriginPoint(origin);
		m_cursorActor->update();
		BuildTime.Modified();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::ReleaseGraphicsResources(
	vtkWindow* w)
{
	if (m_cursorActor && m_cursorVisibility)
	{
		m_cursorActor->getActor()->ReleaseGraphicsResources(w);
	}
}

//-----------------------------------------------------------------------------
int asclepios::gui::vtkResliceWidgetRepresentation::RenderOverlay(
	vtkViewport* viewport)
{
	int count = 0;
	if (m_cursorActor->getActor() && m_cursorVisibility)
	{
		count += m_cursorActor->getActor()->RenderOverlay(viewport);
	}
	return count;
}

//-----------------------------------------------------------------------------
int asclepios::gui::vtkResliceWidgetRepresentation::RenderOpaqueGeometry(
	vtkViewport* viewport)
{
	if (m_cursorActor->getActor() &&
		m_cursorActor->getActor()->GetVisibility() &&
		m_cursorVisibility)
	{
		m_cursorActor->getActor()->RenderOpaqueGeometry(viewport);
	}
	else
	{
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------------
int asclepios::gui::vtkResliceWidgetRepresentation::HasTranslucentPolygonalGeometry()
{
	BuildRepresentation();
	return m_cursorActor->getActor()
		       ? m_cursorActor->getActor()->HasTranslucentPolygonalGeometry()
		       : 0;
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::rotate(const double t_angle)
{
	m_rotationAngle += t_angle;
	m_cursorActor->getActor()->RotateZ(vtkMath::DegreesFromRadians(t_angle));
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::setPlane(const int t_plane)
{
	m_plane = t_plane;
	double verticalColor[3];
	double horizontalColor[3];

	horizontalColor[0] = 255;
	horizontalColor[1] = t_plane == 0 || t_plane == 1 ? 255 : 0;
	horizontalColor[2] = 0;
	verticalColor[0] = t_plane == 1 || t_plane == 2 ? 0 : 255;
	verticalColor[1] = 0;
	verticalColor[2] = t_plane == 1 || t_plane == 2 ? 255 : 0;
	if (m_cursorActor)
	{
		m_cursorActor->createColors(verticalColor, horizontalColor);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkResliceWidgetRepresentation::setCursorPosition(double* t_position)
{
	m_centerMovementPointRepresentation->SetWorldPosition(t_position);
	BuildRepresentation();
}