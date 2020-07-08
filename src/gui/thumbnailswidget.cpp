#include "thumbnailswidget.h"
#include <QPainter>

asclepios::gui::ThumbnailsWidget::ThumbnailsWidget(QWidget* parent)
	: QWidget(parent)
{
	initView();
	initData();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::initView()
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::initData()
{
	m_patientsTabs = new QTabWidget(this);
	m_patientsTabs->setTabPosition(QTabWidget::West);
	m_patientsTabs->setObjectName("ThumbnailsPatients");
	layout()->addWidget(m_patientsTabs);
}

//-----------------------------------------------------------------------------
void asclepios::gui::ThumbnailsWidget::addThumbnail(core::Patient* t_patient)
{

}
