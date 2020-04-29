#include "studylist.h"

asclepios::gui::StudyList::StudyList(QWidget *parent)
	: QListView(parent)
{
	m_ui.setupUi(this);
}
