#include "widget2d.h"


asclepios::gui::Widget2D::Widget2D(QWidget* parent)
	: WidgetBase(parent)
{
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

}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::render()
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::propagateFocusFromChild()
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetView()
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setWindowLevel(const int& t_window, const int& t_level)
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setSliderValues(const int& t_min, const int& t_max, const int& t_value)
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::focusInEvent(QFocusEvent* event)
{
}
