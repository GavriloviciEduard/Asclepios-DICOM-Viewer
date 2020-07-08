#include "series.h"
#include <algorithm>

void asclepios::core::Series::addSingleFrameImage(std::unique_ptr<Image> t_image)
{
	if (!imageAlreadyInserted(m_singleFrameImages, t_image.get()))
	{
		m_singleFrameImages.emplace(std::move(t_image));
	}
}

//-----------------------------------------------------------------------------
void asclepios::core::Series::addMultiFrameImage(std::unique_ptr<Image> t_image)
{
	if (!imageAlreadyInserted(m_multiFrameImages, t_image.get()))
	{
		m_multiFrameImages.emplace(std::move(t_image));
	}
}

//-----------------------------------------------------------------------------
bool asclepios::core::Series::isLess(Series* t_lhs, Series* t_rhs)
{
	return t_lhs->getUID() < t_rhs->getUID();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Series::imageAlreadyInserted(
	const std::set<std::unique_ptr<Image>, Image::imageCompare>& t_images, Image* t_image)
{
	auto const it = std::find_if(t_images.begin(), t_images.end(),
		[&](const std::unique_ptr<Image>& image) { return t_image->equal(t_image); });
	return it != t_images.end();
}
