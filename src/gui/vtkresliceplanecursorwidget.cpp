#include "vtkresliceplanecursorwidget.h"
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkWidgetRepresentation.h>
#include "utils.h"
#include "vtkreslicewidgetrepresentation.h"

vtkStandardNewMacro(asclepios::gui::vtkReslicePlaneCursorWidget);


class vtkReslicePlaneCursorCallback final : public vtkCommand
{
public:
	static vtkReslicePlaneCursorCallback* New()
	{
		return new vtkReslicePlaneCursorCallback;
	}

	void Execute(vtkObject*, const unsigned long eventId, void*) override
	{
		switch (eventId)
		{
		case StartInteractionEvent:
			m_resliceWidget->startWidgetInteraction(m_handleNumber);
			break;
		case InteractionEvent:
			m_resliceWidget->widgetInteraction(m_handleNumber);
			break;
		case EndInteractionEvent:
			m_resliceWidget->endWidgetInteraction(m_handleNumber);
			break;
		default:
			break;
		}
	}

	int m_handleNumber = {};
	asclepios::gui::vtkReslicePlaneCursorWidget* m_resliceWidget = {};
};


asclepios::gui::vtkReslicePlaneCursorWidget::vtkReslicePlaneCursorWidget()
{
	initializeWidget();
}


asclepios::gui::vtkReslicePlaneCursorWidget::~vtkReslicePlaneCursorWidget()
{
	RemoveAllObservers();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::initializeWidget()
{
	m_centerMovementWidget = vtkSmartPointer<vtkHandleWidget>::New();
	m_centerMovementWidget->SetParent(this);
	m_centerMovementCallback =
		vtkReslicePlaneCursorCallback::New();
	m_centerMovementCallback->m_handleNumber = 1;
	m_centerMovementCallback->m_resliceWidget = this;
	m_centerMovementWidget->AddObserver(
		vtkCommand::StartInteractionEvent,
		m_centerMovementCallback,
		Priority);
	m_centerMovementWidget->AddObserver(
		vtkCommand::InteractionEvent,
		m_centerMovementCallback,
		Priority);
	m_centerMovementWidget->AddObserver(
		vtkCommand::EndInteractionEvent,
		m_centerMovementCallback,
		Priority);
	m_centerMovementWidget->AddObserver(
		vtkCommand::Move3DEvent,
		m_centerMovementCallback,
		Priority);
	CallbackMapper->SetCallbackMethod(
		vtkCommand::LeftButtonPressEvent,
		vtkWidgetEvent::AddPoint, this,
		leftMouseDownAction);
	CallbackMapper->SetCallbackMethod(
		vtkCommand::MouseMoveEvent,
		vtkWidgetEvent::Move, this,
		moveMouse);
	CallbackMapper->SetCallbackMethod(
		vtkCommand::LeftButtonReleaseEvent,
		vtkWidgetEvent::EndSelect,
		this, leftMouseUpAction);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::SetEnabled(const int enabling)
{
	if (enabling) //----------------
	{
		vtkDebugMacro(<< "Enabling widget");
		if (Enabled)
		{
			return;
		}
		if (!Interactor)
		{
			vtkErrorMacro(<< "The interactor must be set prior to enabling the widget");
			return;
		}
		if (m_plane == -1)
		{
			return;
		}
		const int X = Interactor->GetEventPosition()[0];
		const int Y = Interactor->GetEventPosition()[1];
		if (!CurrentRenderer)
		{
			SetCurrentRenderer(Interactor->FindPokedRenderer(X, Y));

			if (!CurrentRenderer)
			{
				return;
			}
		}

		Enabled = 1;
		CreateDefaultRepresentation();
		WidgetRep->SetRenderer(CurrentRenderer);

		m_centerMovementWidget->SetRepresentation(
			reinterpret_cast<vtkResliceWidgetRepresentation*>(WidgetRep)->
			getCenterMovementRepresentation());
		reinterpret_cast<vtkResliceWidgetRepresentation*>(WidgetRep)->setPlane(m_plane);
		m_centerMovementWidget->SetInteractor(Interactor);
		m_centerMovementWidget->GetRepresentation()->SetRenderer(CurrentRenderer);
		m_centerMovementWidget->SetEnabled(1);
		m_centerMovementWidget->ManagesCursorOff();
		if (!Parent)
		{
			EventTranslator->AddEventsToInteractor(Interactor,
			                                       EventCallbackCommand, Priority);
		}
		else
		{
			EventTranslator->AddEventsToParent(Parent,
			                                   EventCallbackCommand, Priority);
		}
		if (ManagesCursor)
		{
			WidgetRep->ComputeInteractionState(X, Y);
			SetCursor(WidgetRep->GetInteractionState());
		}
		WidgetRep->BuildRepresentation();
		CurrentRenderer->AddViewProp(WidgetRep);
		InvokeEvent(vtkCommand::EnableEvent, nullptr);
	}
	else //disabling------------------
	{
		vtkDebugMacro(<< "Disabling widget");

		if (!Enabled)
		{
			return;
		}
		Enabled = 0;
		if (!Parent)
		{
			Interactor->RemoveObserver(EventCallbackCommand);
		}
		else
		{
			Parent->RemoveObserver(EventCallbackCommand);
		}
		CurrentRenderer->RemoveViewProp(WidgetRep);
		InvokeEvent(vtkCommand::DisableEvent, nullptr);
		SetCurrentRenderer(nullptr);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::CreateDefaultRepresentation()
{
	if (!WidgetRep)
	{
		WidgetRep = vtkResliceWidgetRepresentation::New();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::leftMouseDownAction(vtkAbstractWidget* w)
{
	vtkReslicePlaneCursorWidget* self =
		reinterpret_cast<vtkReslicePlaneCursorWidget*>(w);
	const int X = self->Interactor->GetEventPosition()[0];
	const int Y = self->Interactor->GetEventPosition()[1];
	auto* const rep =
		dynamic_cast<vtkResliceWidgetRepresentation*>(self->WidgetRep);
	if (!rep)
	{
		self->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
	}
	vtkNew<vtkCellPicker> picker;
	picker->SetTolerance(0.01);
	picker->InitializePickList();
	picker->PickFromListOn();
	const auto resliceActor =
		rep->getResliceActor();
	if (resliceActor && rep->GetVisibility())
	{
		picker->AddPickList(resliceActor->getActor());
	}
	if (picker->Pick(X, Y, 0, self->GetCurrentRenderer()))
	{
		if (self->m_state != translate)
		{
			if (picker->GetActor() == resliceActor->getActor())
			{
				self->m_state = rotate;
			}
		}
		self->InvokeEvent(qualityLow, &self->m_plane);
		self->GrabFocus(self->EventCallbackCommand);
		self->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
		self->EventCallbackCommand->SetAbortFlag(1);
	}
	else
	{
		self->GrabFocus(self->EventCallbackCommand);
		self->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
	}
	self->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::moveMouse(vtkAbstractWidget* w)
{
	auto* const self = reinterpret_cast<vtkReslicePlaneCursorWidget*>(w);
	auto* const rep = dynamic_cast<vtkResliceWidgetRepresentation*>(self->WidgetRep);
	const auto x = self->Interactor->GetEventPosition()[0];
	const auto y = self->Interactor->GetEventPosition()[1];
	const auto lastX = self->Interactor->GetLastEventPosition()[0];
	const auto lastY = self->Interactor->GetLastEventPosition()[1];
	vtkNew<vtkCoordinate> actorCoordinates;
	switch (self->m_state)
	{
	case start:
		self->SetCursor(self->WidgetRep->ComputeInteractionState(x, y));
		if (!self->m_centerMovementWidget->GetEnabled())
		{
			rep->BuildRepresentation();
			self->m_centerMovementWidget->SetEnabled(1);
		}
		break;
	case rotate:
		{
			double* centerActor = rep->getResliceActor()->getActor()->GetPosition();
			actorCoordinates->SetCoordinateSystemToWorld();
			actorCoordinates->SetValue(centerActor[0], centerActor[1]);
			const auto* center = actorCoordinates->GetComputedDisplayValue(self->CurrentRenderer);
			const auto newAngle = atan2(y - center[1], x - center[0]);
			const auto oldAngle = atan2(lastY - center[1], lastX - center[0]);
			self->rotateCursor(newAngle - oldAngle);
			double angle[1] = {vtkMath::DegreesFromRadians(newAngle - oldAngle)};
			self->InvokeEvent(cursorRotate, angle);
		}
		break;
	default:
		break;
	}
	self->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
	self->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::leftMouseUpAction(vtkAbstractWidget* w)
{
	vtkReslicePlaneCursorWidget* self = reinterpret_cast<vtkReslicePlaneCursorWidget*>(w);
	self->SetCursor(0);
	self->m_state = start;
	self->ReleaseFocus();
	self->InvokeEvent(qualityHigh, &self->m_plane);
	self->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
	self->EventCallbackCommand->SetAbortFlag(1);
	self->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::rotateCursor(double t_angle) const
{
	dynamic_cast<vtkResliceWidgetRepresentation*>(WidgetRep)->rotate(t_angle);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::setPlane(int t_plane)
{
	m_plane = t_plane;
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::SetCursor(int t_state)
{
	if (!WidgetRep->GetVisibility())
	{
		return;
	}
	switch (t_state)
	{
	case vtkResliceWidgetRepresentation::outside:
		RequestCursorShape(VTK_CURSOR_DEFAULT);
		break;
	case vtkResliceWidgetRepresentation::mprCursor:
		RequestCursorShape(VTK_CURSOR_HAND);
		break;
	case vtkResliceWidgetRepresentation::handleCursor:
		RequestCursorShape(VTK_CURSOR_SIZEALL);
		break;
	default:
		RequestCursorShape(VTK_CURSOR_ARROW);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::setCursorPosition(double* t_position) const
{
	dynamic_cast<vtkResliceWidgetRepresentation*>
		(WidgetRep)->setCursorPosition(t_position);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::setCursorCenterPosition(const double* t_position)
{
	m_cursorCenterPosition[0] = t_position[0];
	m_cursorCenterPosition[1] = t_position[1];
	m_cursorCenterPosition[2] = t_position[2];
}

double* asclepios::gui::vtkReslicePlaneCursorWidget::getCursorCenterPosition()
{
	return m_cursorCenterPosition;
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::startWidgetInteraction(int handleNum)
{
	Superclass::StartInteraction();
	m_state = translate;
	InvokeEvent(vtkCommand::StartInteractionEvent, nullptr);
	auto* const representation =
		dynamic_cast<vtkResliceWidgetRepresentation*>(WidgetRep);
	lastCursorPos[0] = representation->getResliceActor()->getActor()->GetPosition()[0];
	lastCursorPos[1] = representation->getResliceActor()->getActor()->GetPosition()[1];
	lastCursorPos[2] = representation->getResliceActor()->getActor()->GetPosition()[2];
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::widgetInteraction([[maybe_unused]] int handleNum)
{
	InvokeEvent(vtkCommand::InteractionEvent, nullptr);
	auto* const representation =
		dynamic_cast<vtkResliceWidgetRepresentation*>(WidgetRep);
	double pos[3] = {0.00, 0.00, 0.00};
	pos[0] = representation->getResliceActor()->getActor()->GetPosition()[0] - lastCursorPos[0];
	pos[1] = representation->getResliceActor()->getActor()->GetPosition()[1] - lastCursorPos[1];
	pos[2] = representation->getResliceActor()->getActor()->GetPosition()[2] - lastCursorPos[2];
	InvokeEvent(cursorMove, pos);
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkReslicePlaneCursorWidget::endWidgetInteraction([[maybe_unused]] int handleNum)
{
	Superclass::EndInteraction();
	InvokeEvent(vtkCommand::EndInteractionEvent, nullptr);
	InvokeEvent(cursorFinishMovement, &m_plane);
}
