#include "studylist.h"
#include <QDrag>
#include <QFileDialog>
#include <QMetaMethod>
#include <QMimeData>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QJsonDocument>
#include <QJsonObject>
#include "smartdjdecoderregistration.h"

asclepios::gui::StudyList::StudyList(QWidget* parent)
	: QListWidget(parent)
{
	initView();
}


//-----------------------------------------------------------------------------
void asclepios::gui::StudyList::initView()
{
	m_ui.setupUi(this);
	setAcceptDrops(true);
	setFlow(LeftToRight);
	setResizeMode(Adjust);
	setViewMode(IconMode);
	setSpacing(4);
}

//-----------------------------------------------------------------------------
void asclepios::gui::StudyList::insertNewSeries(core::Series* t_series, core::Image* t_image)
{
	auto* newSeriesItem = new SeriesItem(this);
	newSeriesItem->setSizeHint(QSize(155, 200));
	newSeriesItem->setText(getDescription(m_study, t_series));
	newSeriesItem->setData(Qt::UserRole, createMimeData(t_series, t_image));
	qRegisterMetaType<QVector<int>>("QVector<int>");
	m_futures.push_back(QtConcurrent::run(createImageForItem, this, t_image, newSeriesItem));
	if (!isSignalConnected(QMetaMethod::fromSignal(&StudyList::finishConcurrent)))
	{
		Q_UNUSED(connect(this, &StudyList::finishConcurrent, this, &StudyList::cleanUp))
	}
}

//-----------------------------------------------------------------------------
QString asclepios::gui::StudyList::getDescription(core::Study* t_study,
                                                  core::Series* t_series) const
{
	QString description;
	description.append(QString::fromLatin1(t_study->getDescription().c_str()))
	           .append('\n').append(QString::fromLatin1(t_series->getDescription().c_str()));
	return description;
}

//-----------------------------------------------------------------------------
QString asclepios::gui::StudyList::createMimeData(core::Series* t_series, core::Image* t_image)
{
	QString series;
	QString image;
	series.sprintf("%16p", t_series);
	image.sprintf("%16p", t_image);
	QJsonDocument data;
	QJsonObject obj;
	obj.insert("Series", series);
	obj.insert("Image", image);
	data.setObject(obj);
	return data.toJson();
}

//-----------------------------------------------------------------------------
void asclepios::gui::StudyList::createImageForItem(StudyList* t_self, core::Image* t_image, SeriesItem* t_item)
{
	try
	{
		t_item->setWidth(t_image->getColumns());
		t_item->setHeight(t_image->getRows());
		core::SmartDJDecoderRegistration::registerCodecs();
		const auto dcmImage = std::make_unique<DicomImage>(t_image->getImagePath().c_str(),
			CIF_UsePartialAccessToPixelData | CIF_AcrNemaCompatibility,
			0, 1);
		dcmImage->setWindow(t_image->getWindowCenter(), t_image->getWindowWidth());
		auto* scaledImage = dcmImage->createScaledImage(300ul);
		const auto thpath = t_image->getImagePath() + ".bmp";
		if (scaledImage)
		{
			scaledImage->writeBMP(thpath.c_str(), 24);
			t_item->setSeriesPhoto(QPixmap(thpath.c_str()));
			QFile::remove(QString(thpath.c_str()));
		}
		delete scaledImage;
		t_self->addItem(t_item);
		emit t_self->finishConcurrent();
	}
	catch (const std::exception& ex)
	{
		emit t_self->finishConcurrent();
		//todo log
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::StudyList::startDrag(Qt::DropActions supportedActions)
{
	auto* const drag = new QDrag(this);
	const auto icon = selectedItems().at(0)->icon();
	drag->setPixmap(icon.pixmap(100, 100));
	auto* const mimeData = new QMimeData();
	mimeData->setText(selectedItems().at(0)->data(Qt::UserRole).toString());
	drag->setMimeData(mimeData);
	drag->exec();
}

//-----------------------------------------------------------------------------
void asclepios::gui::StudyList::cleanUp()
{
	core::SmartDJDecoderRegistration::cleanup();
}
