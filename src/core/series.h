#pragma once

#include <unordered_set>
#include <memory>
#include "image.h"

namespace asclepios::core
{
	class Series
	{
	public:
		Series() = default;
		~Series() = default;

		std::unordered_set<std::unique_ptr<Image>>& getImages() { return m_images; }

	private:
		std::unordered_set<std::unique_ptr<Image>> m_images = {};
	};
}
