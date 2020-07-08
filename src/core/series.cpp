#include "series.h"
#include <algorithm>

asclepios::core::Image* asclepios::core::Series::addSingleFrameImage(std::unique_ptr<Image> t_image, bool& t_newImage)
{
	auto index = findImage(m_singleFrameImages, t_image.get());
	t_newImage = false;
	if (index == m_singleFrameImages.size())
	{
		m_singleFrameImages.emplace(std::move(t_image));
		index = m_singleFrameImages.size() - 1;
		t_newImage = true;
	}
	auto it = m_singleFrameImages.begin();
	std::advance(it, index);
	return it->get();
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::core::Series::addMultiFrameImage(std::unique_ptr<Image> t_image, bool& t_newImage)
{
	auto index = findImage(m_multiFrameImages, t_image.get());
	t_newImage = false;
	if (index == m_multiFrameImages.size())
	{
		m_multiFrameImages.emplace(std::move(t_image));
		index = m_multiFrameImages.size() - 1;
		t_newImage = true;
	}
	auto it = m_multiFrameImages.begin();
	std::advance(it, index);
	return it->get();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Series::isLess(Series* t_lhs, Series* t_rhs)
{
	return t_lhs->getUID() < t_rhs->getUID();
}

//-----------------------------------------------------------------------------
std::size_t asclepios::core::Series::findImage(
	const std::set<std::unique_ptr<Image>, Image::imageCompare>& t_images, Image* t_image)
{
	auto const it = std::find_if(t_images.begin(), t_images.end(),
		[&](const std::unique_ptr<Image>& image) { return t_image->equal(t_image); });
	return std::distance(t_images.begin(), it);
}
