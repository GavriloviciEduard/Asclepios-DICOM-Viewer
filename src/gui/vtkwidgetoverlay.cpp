#include "vtkwidgetoverlay.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <vtkDICOMMetaData.h>
#include <vtkDICOMTag.h>
#include <vtkRenderWindow.h>
#include <vtkTextProperty.h>
#include "utils.h"
#include "vtkwidgetoverlaycallback.h"

asclepios::gui::vtkWidgetOverlay::vtkWidgetOverlay()
{
	createOverlayInCorners();
	readOverlayInfo();
}

//-----------------------------------------------------------------------------
asclepios::gui::vtkWidgetOverlay::~vtkWidgetOverlay()
{
	if (m_render)
	{
		m_render->GetRenderWindow()->RemoveObserver(m_observerTag);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::createOverlay(vtkRenderWindow* t_renderWindow, vtkDICOMMetaData* t_metadata)
{
	if (!m_render)
	{
		return;
	}
	clearOverlay();
	createOverlayInfo(t_metadata);
	createCallback(t_renderWindow);
	positionOverlay();
	for (const auto& corner : m_cornersOfOverlay)
	{
		m_render->AddActor(corner->getTextActor());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::clearOverlay()
{
	for (const auto& overlay : m_cornersOfOverlay)
	{
		overlay->clearOverlaysInfo();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::createOverlayInCorners()
{
	for (int i = 0; i < 4; ++i)
	{
		m_cornersOfOverlay[i] = std::make_unique<CornerOverlay>();
		m_cornersOfOverlay[i]->setNumberOfCorner(i);
		if (!m_textProperty[i])
		{
			m_textProperty[i] = vtkSmartPointer<vtkTextProperty>::New();
		}
		m_textProperty[i]->SetColor(1, 1, 1);
		m_textProperty[i]->SetFontFamilyAsString("Arial");
		m_textProperty[i]->SetFontSize(12);
		m_cornersOfOverlay[i]->setTextActorProperty(m_textProperty[i]);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::readOverlayInfo()
{
	QFile overlays(overlaysInformation);
	if (overlays.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		const QByteArray data = overlays.readAll();
		overlays.close();
		const auto doc = QJsonDocument::fromJson(data);
		const QJsonObject root = doc.object();
		QJsonArray values = root.value("Overlay").toArray();
		for (const auto& value : values)
		{
			m_overlays.emplace_back(std::make_unique<OverlayInfo>());
			m_overlays.back()->setTextBefore(value.toObject()["TextBefore"].toString().toStdString());
			m_overlays.back()->setTextAfter(value.toObject()["TextAfter"].toString().toStdString());
			m_overlays.back()->setTagKey(value.toObject()["TagKey"].toInt());
			m_overlays.back()->setCorner(value.toObject()["Corner"].toInt());
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::positionOverlay()
{
	auto* const size =
		m_render->GetRenderWindow()->GetSize();
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			const auto nr = i * 2 + j;
			const auto [x, y] =
				computeCurrentOverlayPosition(i, j, size);
			setPositionOfOverlayInCorner(x, y, nr);
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::createOverlayInfo(vtkDICOMMetaData* t_metadata)
{
	for (const auto& info : m_overlays)
	{
		vtkDICOMTag::StaticTag tag{};
		tag.Key = info->getTagKey();
		auto tagValue = t_metadata->Get(vtkDICOMTag(tag)).AsString();
		core::Utils::processTagFormat(tag, tagValue);
		auto tagText = info->getTextBefore();
		tagText.append(replaceInvalidCharactersInString(tagValue));
		tagText.append(info->getTextAfter());
		if (!tagText.empty() && tagText != "\n"
			&& tagText != "Series: \n" && tagText != "Zoom: \n")
		{
			m_cornersOfOverlay[info->getCorner()]->setOverlayInfo(
				std::to_string(tag.Key), tagText);
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::setPositionOfOverlayInCorner(const int& x, const int& y, const int& nr)
{
	auto* const actor = getOverlayInCorner(nr);
	actor->SetPosition(x, y);
	switch (nr)
	{
	case 1:
		actor->GetTextProperty()->SetJustificationToLeft();
		actor->GetTextProperty()->SetVerticalJustificationToTop();
		break;
	case 2:
		actor->GetTextProperty()->SetJustificationToRight();
		actor->GetTextProperty()->SetVerticalJustificationToBottom();
		break;
	case 3:
		actor->GetTextProperty()->SetJustificationToRight();
		actor->GetTextProperty()->SetVerticalJustificationToTop();
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::vtkWidgetOverlay::createCallback(vtkRenderWindow* t_renderWindow)
{
	const auto callback =
		vtkSmartPointer<vtkWidgetOverlayCallback>::New();
	callback->setWidget(this);
	m_observerTag = t_renderWindow->AddObserver(vtkCommand::ModifiedEvent, callback);
}

//-----------------------------------------------------------------------------
std::tuple<int, int> asclepios::gui::vtkWidgetOverlay::computeCurrentOverlayPosition(const int& i, const int& j,
                                                                                     const int* size)
{
	const auto x = !i
		               ? static_cast<double>(i * (size[0] - 5) + 10)
		               : static_cast<double>(i * (size[0] - 5) - 10);
	const auto y = !j
		               ? static_cast<double>(j * (size[1] - 5) + 20)
		               : static_cast<double>(j * (size[1] - 5) - 20);
	return std::make_tuple(x, y);
}

//-----------------------------------------------------------------------------
std::string asclepios::gui::vtkWidgetOverlay::replaceInvalidCharactersInString(const std::string& t_string)
{
	std::string s;
	const char* cp = t_string.c_str();
	std::size_t m = t_string.size();
	for (std::size_t n = 0; n < t_string.size(); n++)
	{
		m += static_cast<unsigned char>(*cp++) >> 7;
	}
	s.resize(m);
	cp = t_string.c_str();;
	std::size_t i = 0;
	while (i < m)
	{
		while (i < m && (*cp & 0x80) == 0)
		{
			s[i++] = *cp++;
		}
		if (i < m)
		{
			const int code = static_cast<unsigned char>(*cp++);
			s[i++] = (0xC0 | (code >> 6));
			s[i++] = (0x80 | (code & 0x3F));
		}
	}
	return s;
}
