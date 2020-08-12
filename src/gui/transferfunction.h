#pragma once

#include <vector>
#include <memory>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkSmartPointer.h>

class QJsonArray;
class QString;

namespace asclepios::gui
{
	class Color
	{
	public:
		Color() = default;
		~Color() = default;

		//getters
		[[nodiscard]] int getValue() const { return m_value; }
		[[nodiscard]] double getRed() const { return m_red; }
		[[nodiscard]] double getGreen() const { return m_green; }
		[[nodiscard]] double getBlue() const { return m_blue; }

		//setters
		void setValue(const int& t_value) { m_value = t_value; }
		void setRed(const double& t_red) { m_red = t_red; }
		void setGreen(const double& t_green) { m_green = t_green; }
		void setBlue(const double& t_blue) { m_blue = t_blue; }

	private:
		int m_value = {};
		double m_red = {};
		double m_green = {};
		double m_blue = {};
	};

	class Opacity
	{
	public:
		Opacity() = default;
		~Opacity() = default;

		//getters
		[[nodiscard]] int getValue() const { return m_value; }
		[[nodiscard]] double getAlpha() const { return m_alpha; }

		//setters
		void setValue(const int& t_value) { m_value = t_value; }
		void setAlpha(const double& t_alpha) { m_alpha = t_alpha; }

	private:
		int m_value = {};
		double m_alpha = {};
	};
	
	class TransferFunction
	{
	public:
		TransferFunction() = default;
		~TransferFunction() = default;

		//getters
		[[nodiscard]] int getWindow() const { return m_window; }
		[[nodiscard]] int getLevel() const { return m_level; }
		[[nodiscard]] double getAmbient() const { return m_ambient; }
		[[nodiscard]] double getDiffuse() const { return m_diffuse; }
		[[nodiscard]] double getSpecular() const { return m_specular; }
		[[nodiscard]] double getSpecularPower() const { return m_specularPower; }
		[[nodiscard]] bool getHasShade() const { return m_shade; }
 		[[nodiscard]] vtkColorTransferFunction* getColorFunction() const { return m_colorFunction ? m_colorFunction : nullptr; }
		[[nodiscard]] vtkPiecewiseFunction* getOpacityFunction() const { return m_opacityFunction ? m_opacityFunction : nullptr; }

		//setters
		void setWindow(const int& t_window) { m_window = t_window; }
		void setLevel(const int& t_level) { m_level = t_level; }
		void setAmbient(const double& t_ambient) { m_ambient = t_ambient; }
		void setDiffuse(const double& t_diffuse) { m_diffuse = t_diffuse; }
		void setSpecular(const double& t_specular) { m_specular = t_specular; }
		void setSpecularPower(const double& t_specularPower) { m_specularPower = t_specularPower; }
		void setHasShade(const bool& t_shade) { m_shade = t_shade; }
		void setMaximumIntensityProjectionFunction(const int& t_windowCenter, const int& t_windowWidth);

		void updateWindowLevel(const double& t_window, const double& t_level);
		void loadFilterFromFile(const QString& t_fileName);
		

	private:
		int m_window = {};
		int m_level = {};
		double m_ambient = 0.1;
		double m_diffuse = 0.9;
		double m_specular = 1.0;
		double m_specularPower = 64;
		bool m_shade = true;
		std::vector<std::unique_ptr<Color>> m_colors = {};
		std::vector<std::unique_ptr<Opacity>> m_opacities = {};
		vtkSmartPointer<vtkColorTransferFunction> m_colorFunction = {};
		vtkSmartPointer<vtkPiecewiseFunction> m_opacityFunction = {};
		

		void extractColorFunctionInfo(const QJsonArray& t_array);
		void extractOpacityFunctionInfo(const QJsonArray& t_array);
		
	};
}
