#pragma once

#include "ui_generaltoolbar.h"

namespace asclepios::gui
{
	class GeneralToolbar final : public QWidget
	{
	Q_OBJECT

	public:
		explicit GeneralToolbar(QWidget* parent = Q_NULLPTR);
		~GeneralToolbar() = default;

		//getters
		[[nodiscard]] Ui::GeneralToolbar getUI() const { return m_ui; }

	private:
		Ui::GeneralToolbar m_ui = {};

		void initView();
	};
}
