#pragma once

#include <vtkDICOMReader.h>
#include <vtkImageResliceToColors.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

namespace asclepios::core
{
	class Image;
	class Series;
}

namespace asclepios::gui
{
	class MPRMaker
	{
	public:
		MPRMaker() { initialize(); }
		~MPRMaker() = default;

		void setInitialWindow(const int& t_window) { m_initialWindow = t_window; }
		void setInitialLevel(const int& t_level) { m_initialLevel = t_level; }
		void setSeries(core::Series* t_series) { m_series = t_series; }
		void setImage(core::Image* t_image) { m_image = t_image; }
		void SetRenderWindows(
			const vtkSmartPointer<vtkRenderWindow>& t_sagittalWindow,
			const vtkSmartPointer<vtkRenderWindow>& t_coronalWindow,
			const vtkSmartPointer<vtkRenderWindow>& t_axialWindow);

		//getters
		[[nodiscard]] core::Series* getSeries() const { return m_series; }
		[[nodiscard]] core::Image* getImage() const { return m_image; }
		[[nodiscard]] int getInitialWindow() const { return m_initialWindow; }
		[[nodiscard]] int getInitalLevel() const { return m_initialLevel; }
		[[nodiscard]] vtkImageResliceToColors* getImageReslice(const int t_plane) const { return m_reslicer[t_plane]; }
		[[nodiscard]] vtkImageReslice* getOriginalValueImageReslice(int t_plane);
		[[nodiscard]] vtkImageData* getInputData() const { return m_reader->GetOutput(); }
		[[nodiscard]] double getCenterSliceZPosition(int t_plane) const;
		[[nodiscard]] vtkSmartPointer<vtkScalarsToColors> getColorMapScalar() const { return m_colorMap; }


		void create3DMatrix();
		void createMPR();
		void resetMatrixesToInitialPosition();
		void resetWindowLevel();

	private:
		core::Series* m_series = {};
		core::Image* m_image = {};
		int m_initialWindow = 0;
		int m_initialLevel = 0;
		vtkSmartPointer<vtkDICOMReader> m_reader = {};
		vtkSmartPointer<vtkImageResliceToColors> m_reslicer[3] = {};
		vtkSmartPointer<vtkImageReslice> m_originalValuesReslicer[3] = {};
		vtkSmartPointer<vtkRenderWindow> m_renderWindow[3] = {};
		vtkSmartPointer<vtkScalarsToColors> m_colorMap = {};

		double m_sagittalMatrix[16] = {
			0, 0, 1, 0,
			-1, 0, 0, 0,
			0, -1, 0, 0,
			0, 0, 0, 1
		};
		double m_coronalMatrix[16] = {
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, -1, 0, 0,
			0, 0, 0, 1
		};
		double m_axialMatrix[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		void initialize();
		void setInitialMatrix();
		void createMprViews();
		void setMiddleSlice(int t_plane, const vtkSmartPointer<vtkDICOMReader>& t_reader);
		void renderPlaneOffScreen(int t_plane);
	};
}
