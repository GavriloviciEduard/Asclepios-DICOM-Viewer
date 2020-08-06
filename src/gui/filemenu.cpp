#include "filemenu.h"
#include "guiframe.h"

asclepios::gui::FileMenu::FileMenu(QWidget *parent)
	: QMenu(parent)
{
	initView();
	createConnections(parent);
}

//-----------------------------------------------------------------------------
void asclepios::gui::FileMenu::onActionTriggered(QAction* t_action)
{
	const auto action = t_action->objectName();
	if(action == "actionOpenFile")
	{
		emit openFile();
	}
	else if(action == "actionOpenFolder")
	{
		emit openFolder();
	}
	else if(action == "actionCloseAllPatients")
	{
		emit closeAllPatients();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::FileMenu::initView()
{
	m_ui.setupUi(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::FileMenu::createConnections(QWidget* parent) const
{
	Q_UNUSED(connect(this, &QMenu::triggered,
		this, &FileMenu::onActionTriggered));
	auto* const receiver = dynamic_cast<GUIFrame*>(parent);
	Q_UNUSED(connect(this, &FileMenu::openFile,
		receiver, &GUIFrame::onOpenFile));
	Q_UNUSED(connect(this, &FileMenu::openFolder,
		receiver, &GUIFrame::onOpenFolder));
	Q_UNUSED(connect(this, &FileMenu::closeAllPatients,
		receiver, &GUIFrame::onCloseAllPatients));
}
