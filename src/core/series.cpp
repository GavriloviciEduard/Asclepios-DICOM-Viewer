#include "series.h"


void asclepios::core::Series::addSingleFrameImage(std::unique_ptr<Image> t_image)
{
	t_image->setParentObject(this);
	m_singleFrameImages.emplace(std::move(t_image));
}

//-----------------------------------------------------------------------------
void asclepios::core::Series::addMultiFrameImage(std::unique_ptr<Image> t_image)
{
	t_image->setParentObject(this);
	m_multiFrameImages.emplace(std::move(t_image));
}

//-----------------------------------------------------------------------------
bool asclepios::core::Series::isLess(Series* t_lhs, Series* t_rhs)
{
	return t_lhs->getParentObject() == t_rhs->getParentObject() &&
		t_lhs->getUID() == t_rhs->getUID() && t_lhs->getDescription()
		== t_rhs->getDescription() && t_lhs->getDate() ==
		t_rhs->getDate() && t_lhs->getNumber() == t_rhs->getNumber();
}
