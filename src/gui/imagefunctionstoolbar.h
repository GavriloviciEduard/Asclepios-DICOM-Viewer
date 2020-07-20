#pragma once

#include "ui_imagefunctionstoolbar.h"

namespace asclepios::gui
{
	class ImageFunctionsToolbar final : public QWidget
	{
	Q_OBJECT
	public:
		explicit ImageFunctionsToolbar(QWidget* parent = Q_NULLPTR);
		~ImageFunctionsToolbar() = default;

	private:
		Ui::ImageFunctionsToolbar m_ui;

		void initView();
	};
}
