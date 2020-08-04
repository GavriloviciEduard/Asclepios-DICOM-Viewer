#include "widgetbase.h"
#include <QFocusEvent>
#include <QThread>

asclepios::gui::WidgetBase::WidgetBase(QWidget* t_parent)
	: QWidget(t_parent), m_tabWidget(t_parent)
{
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetBase::stopLoadingAnimation()
{
	if(m_loadingAnimation)
	{
		m_loadingAnimation->hide();
		m_loadingAnimation->close();
		m_loadingAnimation.reset();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetBase::setIndexes(const int& t_patientIndex, const int& t_studyIndex,
                                            const int& t_seriesIndex, const int& t_imageIndex)
{
	m_patientIndex = t_patientIndex;
	m_studyIndex = t_studyIndex;
	m_seriesIndex = t_seriesIndex;
	m_imageIndex = t_imageIndex;
}

//-----------------------------------------------------------------------------
void asclepios::gui::WidgetBase::focusInEvent(QFocusEvent* event)
{
	m_tabWidget->setFocus(event->reason());
	QWidget::focusInEvent(event);
}