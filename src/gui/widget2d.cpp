#include "widget2d.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QFocusEvent>
#include <QHBoxLayout>


asclepios::gui::Widget2D::Widget2D(QWidget* parent)
	: WidgetBase(parent)
{
	initData();
	initView();
	createActivationConnections();
	m_tabWidget = parent;
}

//-----------------------------------------------------------------------------
asclepios::gui::Widget2D::~Widget2D()
{
	m_series = nullptr;
	m_image = nullptr;
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
	delete m_scroll;
	delete m_qtvtkWidget;
	m_scroll = new QScrollBar(Qt::Vertical, this);
	m_scroll->setVisible(false);
	m_renderWindow[0] =
		vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_qtvtkWidget = new QVTKOpenGLNativeWidget(this);
	m_qtvtkWidget->SetRenderWindow(m_renderWindow[0]);
	m_vtkEvents = std::make_unique<vtkEventFilter>(this);
	//todo init vtk widget
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::render()
{
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
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setWindowLevel(const int& t_window, const int& t_level)
{
	//todo
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setSliderValues(const int& t_min, const int& t_max, const int& t_value)
{
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
void asclepios::gui::Widget2D::connectScroll()
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::disconnectScroll()
{
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
