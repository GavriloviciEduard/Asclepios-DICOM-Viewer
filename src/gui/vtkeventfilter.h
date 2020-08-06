#pragma once

#include <QObject>

namespace asclepios::gui
{
	class vtkEventFilter final : public QObject
	{
		Q_OBJECT
	public:
		explicit vtkEventFilter(QObject* t_parent) : QObject(t_parent) {};
		~vtkEventFilter() = default;

		signals:
			void activateWidget(const bool& t_flag);
			void setMaximized();

	protected:
		bool eventFilter(QObject* t_watched, QEvent* t_event) override;
	};
}
