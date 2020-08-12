#pragma once

#include <vtkVolume.h>
#include <vtkBoxWidget2.h>
#include <vtkSmartVolumeMapper.h>
#include "transferfunction.h"
#include "vtkboxwidget3dcallback.h"
#include "vtkwidget3dinteractorstyle.h"
#include "vtkwidgetbase.h"

namespace asclepios::gui
{
	class vtkWidget3D final : public vtkWidgetBase
	{
	public:
		vtkWidget3D() { initWidget(); };
		~vtkWidget3D() = default;

		//getters
		[[nodiscard]] vtkSmartVolumeMapper* getvtkWidget3DSmartVolumeMapper() const { return m_mapper; }

		//setters
		void setInteractor(const vtkSmartPointer<vtkRenderWindowInteractor>& t_interactor) override { m_interactor = t_interactor; }
		void setFilter(const QString& t_filePath);
		void render() override;
		void activateBoxWidget(const bool& t_flag);
		void updateFilter() const;

	private:
		std::unique_ptr<TransferFunction> m_transferFunction = {};
		vtkSmartPointer<vtkWidget3DInteractorStyle> m_interactorStyle = {};
		vtkSmartPointer<vtkSmartVolumeMapper> m_mapper;
		vtkSmartPointer<vtkRenderer> m_renderer;
		vtkSmartPointer<vtkVolume> m_volume;
		vtkSmartPointer<vtkBoxWidget2> m_boxWidget;
		vtkSmartPointer<vtkBoxWidget3DCallback> m_boxWidgetCallback = {};
		vtkRenderWindowInteractor* m_interactor = {};

		void initWidget();
		void initBoxWidget();
		void initBoxWidgetCallback();
		void initInteractorStyle();
		void setVolumeMapperBlend() const;
		[[nodiscard]] std::tuple< int, int> getWindowLevel() const;
	};
}
