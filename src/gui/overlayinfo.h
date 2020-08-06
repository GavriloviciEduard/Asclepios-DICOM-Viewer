#pragma once
#include <string>

namespace asclepios::gui
{
	class OverlayInfo
	{
	public:
		OverlayInfo() = default;
		~OverlayInfo() = default;

		//getters
		[[nodiscard]] std::string getTextBefore() const { return m_textBefore; }
		[[nodiscard]] std::string getTextAfter() const { return m_textAfter; }
		[[nodiscard]] unsigned int getTagKey() const { return m_tagKey; }
		[[nodiscard]] int getCorner() const { return m_corner; }

		//setters
		void setTextBefore(const std::string& t_text) { m_textBefore = t_text; }
		void setTextAfter(const std::string& t_text) { m_textAfter = t_text; }
		void setTagKey(const unsigned long& t_tagKey) { m_tagKey = t_tagKey; }
		void setCorner(const int& t_corner) { m_corner = t_corner; }

	private:
		std::string m_textAfter;
		std::string m_textBefore;
		unsigned int m_tagKey = 0;
		int m_corner = 0;
	};
}
