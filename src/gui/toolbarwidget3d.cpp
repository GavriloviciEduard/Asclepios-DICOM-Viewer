#include "toolbarwidget3d.h"
#include "utils.h"
#include <qdir.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QToolButton>


asclepios::gui::ToolbarWidget3D::ToolbarWidget3D(QWidget* parent)
	: QWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidget3D::onFilterChanged(const int& t_index)
{
	const auto path =
		m_ui.comboBoxFilters->itemData(t_index);
	if (path.isValid())
	{
		emit filterChanged(path.toString());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidget3D::onCropPressed()
{
	emit cropPressed(dynamic_cast<QToolButton*>
		(sender())->isChecked());
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidget3D::initView()
{
	m_ui.setupUi(this);
	initCombo();
}

//-----------------------------------------------------------------------------
void asclepios::gui::ToolbarWidget3D::initCombo() const
{
	QDir dir(filters3dDir);
	dir.setNameFilters(QStringList() << "*.json");
	const auto files =
		dir.entryInfoList();
	for (const auto& file : files)
	{
		QFile currentFile(file.absoluteFilePath());
		if (!currentFile.open(QIODevice::ReadOnly
			| QIODevice::Text))
		{
			continue;
		}
		const QByteArray data =
			currentFile.readAll();
		currentFile.close();
		const auto doc =
			QJsonDocument::fromJson(data);
		const QJsonObject root =
			doc.object();
		if (!root.empty())
		{
			m_ui.comboBoxFilters->addItem(
				root.value("name").toString());
			m_ui.comboBoxFilters->setItemData(
				m_ui.comboBoxFilters->count() - 1,
				file.absoluteFilePath());
		}
	}
	m_ui.comboBoxFilters->addItem("MIP");
	m_ui.comboBoxFilters->setItemData(
		m_ui.comboBoxFilters->count() - 1, "MIP");
}
