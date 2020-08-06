#pragma once
#include <memory>


#include "widgetscontainer.h"
#include "widgetsrepository.h"

namespace asclepios::gui
{
	class FilesImporter;
	class WidgetsController final : public QObject
	{
	Q_OBJECT
	public:
		WidgetsController();
		~WidgetsController() = default;


		//getters
		[[nodiscard]] WidgetsRepository* getWidgetsRepository() const { return m_widgetsRepository.get(); }
		[[nodiscard]] WidgetsContainer* getWidgetsContainer() const { return m_widgetsContainer.get(); }
		[[nodiscard]] TabWidget* getActiveWidget() const { return m_activeWidget; }

		//setters
		void setFilesImporter(FilesImporter* t_importer) { m_filesImporter = t_importer; }

		void createWidgets(const WidgetsContainer::layouts& t_layout);
		void applyTransformation(const transformationType& t_type) const;
		void resetData() const;
		void waitForRenderingThreads() const;
		
	public slots:
		void setActiveWidget(TabWidget* t_widget);
		void setMaximize(TabWidget* t_widget) const;
		void populateWidget(core::Series* t_series, core::Image* t_image) const;

	private:
		std::unique_ptr<WidgetsRepository> m_widgetsRepository = {};
		std::unique_ptr<WidgetsContainer> m_widgetsContainer = {};
		TabWidget* m_activeWidget = {};
		FilesImporter* m_filesImporter = {};

		void initData();
		void createRemoveWidgets(const std::size_t& t_nrWidgets) const;
		void createConnections() const;
		void resetConnections();
		[[nodiscard]] TabWidget* createNewWidget() const;
		[[nodiscard]] TabWidget* findNextAvailableWidget() const;
		[[nodiscard]] std::size_t computeNumberWidgetsFromLayout(const WidgetsContainer::layouts& t_layout);
	};
}
