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
bool asclepios::core::Series::operator==(const Series& t_rhs) const
{
	return getParentObject() == t_rhs.getParentObject() &&
		getUID() == t_rhs.getUID() && getDescription() ==
		t_rhs.getDescription() && getDate() ==
		t_rhs.getDate() && getNumber() == t_rhs.getNumber();
}
