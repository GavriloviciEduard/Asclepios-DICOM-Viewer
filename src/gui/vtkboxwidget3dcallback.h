#pragma once

#include <vtkCommand.h>
#include <vtkNew.h>
#include <vtkPlanes.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkVolume.h>

namespace asclepios::gui
{
	class vtkBoxWidget3DCallback final : public vtkCommand
	{
	public:
		static vtkBoxWidget3DCallback* New();
		vtkTypeMacro(vtkBoxWidget3DCallback, vtkCommand);
		vtkBoxWidget3DCallback() : m_transform(vtkSmartPointer<vtkTransform>::New()) {}
		~vtkBoxWidget3DCallback() = default;

		//getters
		[[nodiscard]] vtkVolume* getVolume() const { return m_volume; }

		//setters
		void setVolume(vtkVolume* t_volume) { m_volume = t_volume; }

		void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;

	private:
		vtkVolume* m_volume = {};
		vtkNew<vtkPlanes> m_planes;
		vtkSmartPointer<vtkTransform> m_transform = {};
	};
}
