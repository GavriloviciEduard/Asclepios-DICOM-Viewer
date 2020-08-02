#pragma once

#include <QWidget>
#include "ui_loadinganimation.h"

namespace asclepios::gui
{
	class LoadingAnimation final : public QWidget
	{
	Q_OBJECT
	public:
		explicit LoadingAnimation(QWidget* parent = Q_NULLPTR);
		~LoadingAnimation() = default;

		void start();

	private:
		Ui::LoadingAnimation m_ui;

		void initView();
	};
}
