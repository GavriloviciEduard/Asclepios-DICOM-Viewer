#pragma once

#include <QListWidgetItem>

namespace asclepios::gui
{
	class SeriesItem final : public QListWidgetItem
	{
	public:
		explicit SeriesItem(QListWidget* parent) : QListWidgetItem(parent) {}
		~SeriesItem() = default;

		//getters
		[[nodiscard]] unsigned short getWidth() const { return m_width; }
		[[nodiscard]] unsigned short getHeight() const { return m_height; }

		//setters
		void setSeriesPhoto(const QPixmap& t_photo) { setIcon(QIcon(t_photo)); }
		void setWidth(const unsigned short& t_width) { m_width = t_width; }
		void setHeight(const unsigned short& t_height) { m_height = t_height; }

	private:
		unsigned short m_width = 0;
		unsigned short m_height = 0;
	};
}