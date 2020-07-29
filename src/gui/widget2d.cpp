#include "widget2d.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QFocusEvent>
#include <QHBoxLayout>

#include "vtkwidget2dinteractorstyle.h"


asclepios::gui::Widget2D::Widget2D(QWidget* parent)
	: WidgetBase(parent)
{
	initData();
	initView();
	createActivationConnections();
	m_tabWidget = parent;
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::initView()
{
	m_ui.setupUi(this);
	setLayout(new QHBoxLayout(this));
	layout()->setMargin(0);
	layout()->setSpacing(0);
	layout()->addWidget(m_qtvtkWidget);
	layout()->addWidget(m_scroll);
}

void asclepios::gui::Widget2D::initData()
{
	disconnectScroll();
	delete m_scroll;
	delete m_qtvtkWidget;
	m_scroll = new QScrollBar(Qt::Vertical, this);
	m_renderWindow[0] =
		vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_vtkWidget = std::make_unique<vtkWidget2D>();
	m_qtvtkWidget = new QVTKOpenGLNativeWidget(this);
	m_qtvtkWidget->SetRenderWindow(m_renderWindow[0]);
	m_vtkWidget->setRenderWindow(m_qtvtkWidget->GetRenderWindow());
	m_vtkEvents = std::make_unique<vtkEventFilter>(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::render()
{
	//todo
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::createActivationConnections()
{
	setFocusPolicy(Qt::FocusPolicy::WheelFocus);
	m_qtvtkWidget->installEventFilter(m_vtkEvents.get());
	Q_UNUSED(connect(m_vtkEvents.get(),
		&vtkEventFilter::activateWidget,
		this, &Widget2D::activateWidget));
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetView()
{
	resetWidgets();
	resetScroll();
	m_image = nullptr;
	m_series = nullptr;
	//todo reset title of tab
	disconnectScroll();
	createActivationConnections();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setSliderValues(const int& t_min, const int& t_max, const int& t_value)
{
	if (m_scroll)
	{
		const QSignalBlocker blocker(m_scroll);
		m_scroll->setMinimum(t_min);
		m_scroll->setMaximum(t_max);
		m_scroll->setValue(t_value);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::activateWidget(const bool& t_flag)
{
	if (t_flag)
	{
		auto* event = new QFocusEvent(QEvent::FocusIn,
		                              Qt::FocusReason::MouseFocusReason);
		focusInEvent(event);
		delete event;
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::applyTransformation(const transformationType& t_type) const
{
	if (m_vtkWidget && m_image)
	{
		auto* widget2d =
			dynamic_cast<vtkWidget2D*>(m_vtkWidget.get());
		if (widget2d)
		{
			widget2d->applyTransformation(t_type);
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::refreshSliderValues(const int& t_patientIndex, const int& t_studyIndex,
                                                   const int& t_seriesIndex, const int& t_imagesInSeries,
                                                   const int& t_seriesSize)
{
	if (canScrollBeRefreshed(t_patientIndex, t_studyIndex, t_seriesIndex))
	{
		if (!m_image->getIsMultiFrame())
		{
			//todo
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::renderFinished()
{
	//todo
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::changeImage(int& t_index)
{
	try
	{
		auto* interactorStyle =
			dynamic_cast<vtkWidget2DInteractorStyle*>(
				m_qtvtkWidget->GetRenderWindow()->
				               GetInteractor()->GetInteractorStyle());
		if (interactorStyle)
		{
			activateWidget(true);
			interactorStyle->changeImage(t_index);
		}
	}
	catch (const std::exception& ex)
	{
		//todo
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::connectScroll() const
{
	Q_UNUSED(connect(m_scroll, &QScrollBar::valueChanged,
		this, &Widget2D::changeImage));
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::disconnectScroll() const
{
	disconnect(m_scroll, &QScrollBar::valueChanged,
	           this, &Widget2D::changeImage);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetWidgets()
{
	if (m_vtkWidget)
	{
		m_vtkWidget.reset();
	}
	m_vtkWidget = std::make_unique<vtkWidget2D>();
	m_qtvtkWidget->SetRenderWindow(vtkNew<vtkGenericOpenGLRenderWindow>());
	m_renderWindow[0] = m_qtvtkWidget->GetRenderWindow();
	m_vtkWidget->setRenderWindow(m_renderWindow[0]);
	m_renderWindow[0]->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetScroll()
{
	if (m_scroll)
	{
		const QSignalBlocker blocker(m_scroll);
		m_scroll->setValue(0);
		m_scroll->setMaximum(0);
	}
	else
	{
		m_scroll = new QScrollBar(Qt::Vertical, this);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::initImageReader(vtkWidget2D* t_vtkWidget2D, Widget2D* t_self)
{
	t_vtkWidget2D->initImageReader();
	emit t_self->imageReaderInitialized();
}

//-----------------------------------------------------------------------------
bool asclepios::gui::Widget2D::canScrollBeRefreshed(const int& t_patientIndex, const int& t_studyIndex,
                                                    const int& t_seriesIndex) const
{
	return m_vtkWidget && m_vtkWidget->getImage() && t_patientIndex == m_patientIndex && t_patientIndex != -1 &&
		t_studyIndex == m_studyIndex && t_studyIndex != -1 && m_seriesIndex == t_seriesIndex && t_seriesIndex != -1;
}
