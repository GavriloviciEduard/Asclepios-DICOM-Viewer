#include "widgetscontainer.h"

asclepios::gui::WidgetsContainer::WidgetsContainer(QWidget* t_parent)
	: QWidget(t_parent)
{
	initView();
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::setLayout(const layouts& t_layout) const
{
	switch (t_layout)
	{
	case layouts::one:
		one();
		break;
	case layouts::twoRowOneBottom:
		twoRowOneBottom();
		break;
	case layouts::twoColumnOneRight:
		twoColumnOneRight();
		break;
	case layouts::threeRowOneBottom:
		threeRowOneBottom();
		break;
	case layouts::threeColumnOneRight:
		threeColumnOneRight();
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::initView()
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::initData()
{
	m_layoutAsGrid = dynamic_cast<QGridLayout*>(layout());
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::one() const
{
	layout()->addWidget(*&m_widgetsReference->at(0));
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::twoRowOneBottom() const
{
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(1), 0, 1);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(2), 1, 0, 1, 2);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::twoColumnOneRight() const
{
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(1), 1, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(2), 0, 1, 2, 1);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::threeRowOneBottom() const
{
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(1), 0, 1);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(2), 0, 2);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(3), 1, 0, 1, 3);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::threeColumnOneRight() const
{
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(1), 1, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(2), 2, 0);
	m_layoutAsGrid->addWidget(*&m_widgetsReference->at(3), 0, 1, 3, 1);
}
