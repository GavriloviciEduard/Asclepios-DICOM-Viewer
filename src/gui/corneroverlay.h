#pragma once

#include <map>
#include <string>
#include <vtkNew.h>
#include <vtkOpenGLTextActor.h>

namespace asclepios::gui
{
	class CornerOverlay
	{
	public:
		CornerOverlay() = default;
		~CornerOverlay() = default;

		//getters
		[[nodiscard]] int getNumberOfCorner() const { return m_numberOfCorner; }
		[[nodiscard]] vtkOpenGLTextActor* getTextActor();
		[[nodiscard]] std::string getOverlayFromInfo();

		//setters
		void setNumberOfCorner(const int& t_nr) { m_numberOfCorner = t_nr; }
		void setOverlayInfo(const std::string& t_key, const std::string& t_value) { m_overlaysInfo[t_key] = t_value; }
		void setTextActorProperty(vtkTextProperty* t_property) const { m_textActor->SetTextProperty(t_property); }

		void initTextActor() { m_textActor->SetInput(getOverlayFromInfo().c_str()); }
		void clearOverlaysInfo() { m_overlaysInfo.clear(); }

	private:
		std::map<std::string, std::string> m_overlaysInfo = {};
		int m_numberOfCorner = -1;
		vtkNew<vtkOpenGLTextActor> m_textActor;
	};
}
