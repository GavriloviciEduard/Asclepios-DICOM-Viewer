#pragma once

#include <imebra.h>

namespace asclepios::core
{
	class Image
	{
	public:
		Image() = default;
		~Image() = default;

		//getters
		//setters
		void setImage(std::unique_ptr<imebra::Image> t_image) { m_image = std::move(t_image); }


	private:
		std::unique_ptr<imebra::Image> m_image = {};
	};
}
