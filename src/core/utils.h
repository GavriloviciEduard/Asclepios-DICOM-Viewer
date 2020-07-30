#pragma once
#include <vtkCommand.h>
#include <vtkDICOMTag.h>

#define export __declspec(dllexport)
#define unused(x) (void)x;
constexpr auto iconTitleBar = ":/res/icon_small.png";
constexpr auto iconapp = ":/res/icon_big.png";
constexpr auto appBackground = ":/res/background.png";
constexpr auto buttonMaximizeOff = ":/res/maximize-button1.png";
constexpr auto buttonMaximizeOn = ":/res/maximize-button2.png";
constexpr auto overlaysInformation = "overlays.json";

enum class transformationType
{
	none,
	flipHorizontal,
	flipVertical,
	rotateLeft,
	rotateRight,
	invert,
	scrollMouse,
	zoom,
	pan,
	windowLevel
};

enum class overlayKey
{
	zoom = 1001,
	series = 1003,
	window = 2625616,
	level = 2625617
};

enum vtkCustomEvents : unsigned long
{
	imageChanged = vtkCommand::UserEvent + 1,
};

namespace asclepios::core
{
	class export Utils
	{
	public:
		Utils() = default;
		~Utils() = default;

		static void processTagFormat(const vtkDICOMTag& t_tag, std::string& t_value);

	private:
		[[nodiscard]] static std::string getDateFormat(const std::string& t_date);
		[[nodiscard]] static std::string getTimeFormat(const std::string& t_time);
	};
}
