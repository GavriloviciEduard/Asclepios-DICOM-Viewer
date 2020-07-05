#pragma once

#include <QListView>
#include "ui_studylist.h"

namespace asclepios::gui
{
	class StudyList : public QListView
	{
	Q_OBJECT

	public:
		explicit StudyList(QWidget* parent = Q_NULLPTR);
		~StudyList() = default;

	private:
		Ui::StudyList m_ui = {};
	};
}
