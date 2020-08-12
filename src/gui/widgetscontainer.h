#pragma once

#include "tabwidget.h"
#include "ui_widgetscontainer.h"

namespace asclepios::gui
{
	class WidgetsContainer final : public QWidget
	{
	Q_OBJECT
	public:
		explicit WidgetsContainer(QWidget* t_parent = Q_NULLPTR);
		~WidgetsContainer() = default;

		enum class layouts
		{
			none,
			one,
			twoRowOneBottom,
			twoColumnOneRight,
			threeRowOneBottom,
			threeColumnOneRight
		};

		//getters
		Ui::WidgetsContainer getUI() const { return m_ui; }

		//setters
		void setWidgetReference(std::vector<TabWidget*>* t_widgetsReference) {
			m_widgetsReference = t_widgetsReference; }

		void setLayout(const layouts& t_layout) const;
		
	signals:
		void applyTransformation(const transformationType& t_type);
		void closePatients();
		void createWidget3D();

	private slots:
		void onApplyTransformation();
		void onClosePatients();
		void onCreateWidget3D();

	private:
		Ui::WidgetsContainer m_ui = {};
		std::vector<TabWidget*>* m_widgetsReference = {};


		void initView();
		void setProperties() const;

		void one() const;
		void twoRowOneBottom() const;
		void twoColumnOneRight() const;
		void threeRowOneBottom() const;
		void threeColumnOneRight() const;
	};
}
