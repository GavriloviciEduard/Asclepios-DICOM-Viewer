#include "widgetscontainer.h"

asclepios::gui::WidgetsContainer::WidgetsContainer(QWidget* t_parent)
	: QWidget(t_parent)
{
	initView();
	setProperties();
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::onApplyTransformation()
{
	emit applyTransformation(static_cast<transformationType>(sender()
		->property("transformation").toInt()));
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::onClosePatients()
{
	emit closePatients();
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::onCreateWidget3D()
{
	emit createWidget3D();
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
void asclepios::gui::WidgetsContainer::setProperties() const
{
	m_ui.buttonInvert->setProperty("transformation",
		static_cast<int>(transformationType::invert));
	m_ui.buttonFlipHorizontal->setProperty("transformation",
		static_cast<int>(transformationType::flipHorizontal));
	m_ui.buttonFlipVerical->setProperty("transformation",
		static_cast<int>(transformationType::flipVertical));
	m_ui.buttonRotateLeft->setProperty("transformation",
		static_cast<int>(transformationType::rotateLeft));
	m_ui.buttonRotateRight->setProperty("transformation",
		static_cast<int>(transformationType::rotateRight));
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::one() const
{
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(0), 0, 0);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::twoRowOneBottom() const
{
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(1), 0, 1);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(2), 1, 0, 1, 2);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::twoColumnOneRight() const
{
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(1), 1, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(2), 0, 1, 2, 1);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::threeRowOneBottom() const
{
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(1), 0, 1);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(2), 0, 2);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(3), 1, 0, 1, 3);
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetsContainer::threeColumnOneRight() const
{
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(0), 0, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(1), 1, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(2), 2, 0);
	m_ui.gridLayout->addWidget(*&m_widgetsReference->at(3), 0, 1, 3, 1);
}
