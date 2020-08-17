#include "series.h"
#include <algorithm>
#include <vtkDICOMSorter.h>
#include <vtkDICOMMetaData.h>
#include <vtkStringArray.h>

asclepios::core::Image* asclepios::core::Series::getNextSingleFrameImage(Image* t_image)
{
	if (t_image->equal(m_singleFrameImages.rbegin()->get()))
	{
		return t_image;
	}
	const auto index = findImageIndex(m_singleFrameImages, t_image);
	auto it = m_singleFrameImages.begin();
	std::advance(it, index + 1);
	return it->get();
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::core::Series::getPreviousSingleFrameImage(Image* t_image)
{
	if (t_image->equal(m_singleFrameImages.begin()->get()))
	{
		return t_image;
	}
	const auto index = findImageIndex(m_singleFrameImages, t_image);
	auto it = m_singleFrameImages.begin();
	std::advance(it, index - 1);
	return it->get();
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::core::Series::getSingleFrameImageByIndex(const int& t_index)
{
	auto it = m_singleFrameImages.begin();
	std::advance(it, t_index);
	if (it != m_singleFrameImages.end())
	{
		return it->get();
	}
	throw std::runtime_error("Index is out of bounds!");
}

//-----------------------------------------------------------------------------
vtkSmartPointer<vtkDICOMReader> asclepios::core::Series::getReaderForAllSingleFrameImages()
{
	if (m_readerSingleFrame)
	{
		return vtkSmartPointer<vtkDICOMReader>(m_readerSingleFrame);
	}
	vtkNew<vtkDICOMReader> newReader;
	vtkNew<vtkStringArray> sinleFramesImages;
	vtkNew<vtkDICOMSorter> sorter;
	int count = 0;
	for (const auto& image : m_singleFrameImages)
	{
		const auto path = image->getImagePath();
		if (!path.empty())
		{
			sinleFramesImages->InsertValue(count++, path);
		}
	}
	sorter->SetInputFileNames(sinleFramesImages);
	sorter->Update();
	newReader->SetFileNames(sorter->GetFileNamesForSeries(0));
	newReader->SetDataByteOrderToLittleEndian();
	newReader->Update(0);
	m_readerSingleFrame = newReader;
	return vtkSmartPointer<vtkDICOMReader>(newReader);
}

//-----------------------------------------------------------------------------
vtkSmartPointer<vtkDICOMMetaData> asclepios::core::Series::getMetaDataForSeries()
{
	if (m_singleFrameImages.empty())
	{
		return nullptr;
	}
	if (!m_metaDataSingleFrame)
	{
		m_metaDataSingleFrame = vtkSmartPointer<vtkDICOMMetaData>::New();
	}
	vtkNew<vtkDICOMReader> tempReader;
	tempReader->SetFileName((*m_singleFrameImages.begin())->getImagePath().c_str());
	tempReader->Update();
	m_metaDataSingleFrame->DeepCopy(tempReader->GetMetaData());
	return m_metaDataSingleFrame;
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::core::Series::addSingleFrameImage(std::unique_ptr<Image> t_image, bool& t_newImage)
{
	auto index = findImageIndex(m_singleFrameImages, t_image.get());
	t_newImage = false;
	if (index == m_singleFrameImages.size())
	{
		m_singleFrameImages.emplace(std::move(t_image));
		index = m_singleFrameImages.size() - 1;
		t_newImage = true;
	}
	auto it = m_singleFrameImages.begin();
	std::advance(it, index);
	it->get()->setIndex(index);
	return it->get();
}

//-----------------------------------------------------------------------------
asclepios::core::Image* asclepios::core::Series::addMultiFrameImage(std::unique_ptr<Image> t_image, bool& t_newImage)
{
	auto index = findImageIndex(m_multiFrameImages, t_image.get());
	t_newImage = false;
	if (index == m_multiFrameImages.size())
	{
		m_multiFrameImages.emplace(std::move(t_image));
		index = m_multiFrameImages.size() - 1;
		t_newImage = true;
	}
	auto it = m_multiFrameImages.begin();
	std::advance(it, index);
	it->get()->setIndex(index);
	return it->get();
}

//-----------------------------------------------------------------------------
bool asclepios::core::Series::isLess(Series* t_lhs, Series* t_rhs)
{
	return t_lhs->getUID() < t_rhs->getUID();
}

//-----------------------------------------------------------------------------
std::size_t asclepios::core::Series::findImageIndex(
	const std::set<std::unique_ptr<Image>, Image::imageCompare>& t_images, Image* t_image)
{
	auto const it = std::find_if(t_images.begin(), t_images.end(),
	                             [&](const std::unique_ptr<Image>& image) { return image->equal(t_image); });
	return std::distance(t_images.begin(), it);
}
