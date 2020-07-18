#include "widget2d.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QHBoxLayout>


asclepios::gui::Widget2D::Widget2D(QWidget* parent)
	: WidgetBase(parent)
{
	initData();
	initView();
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
void asclepios::gui::Widget2D::focusInEvent(QFocusEvent* event)
{
	//todo
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::activateWidget(const bool& t_flag)
{
	//todo
}
