#include "studylist.h"
#include "seriesitem.h"

asclepios::gui::StudyList::StudyList(QWidget *parent)
	: QListWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::StudyList::initView()
{
	m_ui.setupUi(this);
	setAcceptDrops(true);
	setFlow(LeftToRight);
	setResizeMode(Adjust);
	setViewMode(IconMode);
	setSpacing(2);
}

void asclepios::gui::StudyList::insertNewSeries(core::Series* t_series)
{
	auto* const seriesItem = new SeriesItem(this);
}


