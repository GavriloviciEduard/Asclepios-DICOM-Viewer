#pragma once

#include "ui_loadinganimation.h"
#include <qmovie.h>
#include <qdialog.h>

namespace asclepios::gui
{
	class LoadingAnimation final : public QDialog
	{
	Q_OBJECT
	public:
		explicit LoadingAnimation(QWidget* parent = Q_NULLPTR);
		~LoadingAnimation() = default;

	private:
		Ui::LoadingAnimation m_ui = {};
		QMovie m_movie {};

		void initView();
	};
}
