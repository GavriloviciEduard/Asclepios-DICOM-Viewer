#include "widget2d.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QFocusEvent>
#include <QtConcurrent/QtConcurrent>
#include "tabwidget.h"
#include "vtkwidget2dinteractorstyle.h"
#include "study.h"
#include "patient.h"


asclepios::gui::Widget2D::Widget2D(QWidget* parent)
	: WidgetBase(parent)
{
	initData();
	initView();
	createConnections();
	m_tabWidget = parent;
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::initView()
{
	m_ui.setupUi(this);
	setLayout(new QHBoxLayout(this));
	layout()->setMargin(0);
	layout()->setSpacing(0);
	layout()->addWidget(m_qtvtkWidget);
	layout()->addWidget(m_scroll);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::initData()
{
	disconnectScroll();
	delete m_scroll;
	delete m_qtvtkWidget;
	m_scroll = new QScrollBar(Qt::Vertical, this);
	setScrollStyle();
	m_renderWindow[0] =
		vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_vtkWidget = std::make_unique<vtkWidget2D>();
	m_qtvtkWidget = new QVTKOpenGLNativeWidget(this);
	m_qtvtkWidget->SetRenderWindow(m_renderWindow[0]);
	m_vtkWidget->setRenderWindow(m_qtvtkWidget->GetRenderWindow());
	m_vtkEvents = std::make_unique<vtkEventFilter>(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::render()
{
	if (m_qtvtkWidget && m_vtkWidget && m_renderWindow->Get())
	{
		try
		{
			startLoadingAnimation();
			dynamic_cast<TabWidget*>(m_tabWidget)->setTabTitle(0,
				m_series->getDescription().c_str());
			auto* const vtkWidget = dynamic_cast<vtkWidget2D*>(m_vtkWidget.get());
			vtkWidget->setSeries(m_series);
			vtkWidget->setImage(m_image);
			vtkWidget->resetOverlay();
			m_tabWidget->setAcceptDrops(false);
			m_future = QtConcurrent::run(initImageReader, vtkWidget, this);
			Q_UNUSED(connect(this, &Widget2D::imageReaderInitialized,
				this, &Widget2D::renderFinished));
		}
		catch (std::exception& ex)
		{
			m_future = {};
			//todo log
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::createConnections()
{
	setFocusPolicy(Qt::FocusPolicy::WheelFocus);
	m_qtvtkWidget->installEventFilter(m_vtkEvents.get());
	Q_UNUSED(connect(m_vtkEvents.get(),
		&vtkEventFilter::activateWidget,
		this, &Widget2D::activateWidget));
	Q_UNUSED(connect(m_vtkEvents.get(),
		&vtkEventFilter::setMaximized,
		this, &Widget2D::setMaximized));
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetView()
{
	resetWidgets();
	resetScroll();
	m_isImageLoaded = false;
	m_image = nullptr;
	m_series = nullptr;
	//todo reset title of tab
	disconnectScroll();
	createConnections();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setSliderValues(const int& t_min, const int& t_max, const int& t_value)
{
	if (m_scroll)
	{
		const QSignalBlocker blocker(m_scroll);
		m_scroll->setMinimum(t_min);
		m_scroll->setMaximum(t_max);
		m_scroll->setValue(t_value);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::activateWidget(const bool& t_flag)
{
	if (t_flag)
	{
		auto* event = new QFocusEvent(QEvent::FocusIn,
			Qt::FocusReason::MouseFocusReason);
		focusInEvent(event);
		delete event;
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::applyTransformation(const transformationType& t_type) const
{
	if (m_vtkWidget && m_image)
	{
		auto* widget2d =
			dynamic_cast<vtkWidget2D*>(m_vtkWidget.get());
		if (widget2d)
		{
			widget2d->applyTransformation(t_type);
		}
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::refreshScrollValues(core::Series* t_series)
{
	auto* const study = t_series->getParentObject();
	if (canScrollBeRefreshed(study->getParentObject()->getIndex(),
		study->getIndex(), t_series->getIndex()))
	{
		if (!m_image->getIsMultiFrame())
		{
			const auto size = static_cast<int>(t_series->getSinlgeFrameImages().size());
			setSliderValues(0, size - 1,
				size <= m_scroll->value() ? m_scroll->value() + 1 : m_scroll->value());
			dynamic_cast<vtkWidget2D*>(m_vtkWidget.get())->updateOvelayImageNumber(0,
				size,
				std::stoi(m_series->getNumber()));
		}
	}
}

void asclepios::gui::Widget2D::changeScrollValue(vtkObject* t_obj , unsigned long , void*, void*) const
{
	const QSignalBlocker blocker(m_scroll);
	auto* const  style =
		dynamic_cast<vtkWidget2DInteractorStyle*>(t_obj);
	if(style)
	{
		m_scroll->setValue(style->getCurrentImageIndex());
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::setMaximized() const
{
	dynamic_cast<TabWidget*>(m_tabWidget)->onMaximize();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::renderFinished()
{
	m_vtkWidget->setInteractor(m_qtvtkWidget->
		GetRenderWindow()->GetInteractor());
	m_vtkWidget->render();
	auto const max = m_image->getIsMultiFrame()
		? m_image->getNumberOfFrames() - 1
		: static_cast<int>(m_series->getSinlgeFrameImages().size()) - 1;
	m_scroll->setMaximum(max);
	dynamic_cast<vtkWidget2D*>(m_vtkWidget.get())
		->updateOvelayImageNumber(0, max + 1,
			std::stoi(m_series->getNumber()));
	connectScroll();
	m_scroll->setVisible(m_scroll->maximum() > 1);
	m_tabWidget->setAcceptDrops(true);
	m_future = {};
	disconnect(this, &Widget2D::imageReaderInitialized,
	           this, &Widget2D::renderFinished);
	stopLoadingAnimation();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::closeEvent(QCloseEvent* t_event)
{
	initView();
	initData();
	QWidget::closeEvent(t_event);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::changeImage(int t_index)
{
	try
	{
		auto* interactorStyle =
			dynamic_cast<vtkWidget2DInteractorStyle*>(
				m_qtvtkWidget->GetRenderWindow()->
				GetInteractor()->GetInteractorStyle());
		if (interactorStyle)
		{
			activateWidget(true);
			interactorStyle->changeImage(t_index);
		}
	}
	catch (const std::exception& ex)
	{
		//todo
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::connectScroll()
{
	if (!m_scrollConnection)
	{
		m_scrollConnection = vtkSmartPointer<vtkEventQtSlotConnect>::New();
	}
	m_scrollConnection->Connect(
		m_qtvtkWidget->GetRenderWindow()->GetInteractor()->GetInteractorStyle(),
		vtkCustomEvents::changeScrollValue, this,
		SLOT(changeScrollValue(vtkObject*, unsigned long, void*, void*)));
	Q_UNUSED(connect(m_scroll, &QScrollBar::valueChanged,
		this, &Widget2D::changeImage));
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::disconnectScroll() const
{
	if (m_scroll)
	{
		disconnect(m_scroll, &QScrollBar::valueChanged,
		           this, &Widget2D::changeImage);
	}
	if (m_scrollConnection)
	{
		m_scrollConnection->Disconnect(
			m_qtvtkWidget->GetRenderWindow()
			->GetInteractor()->GetInteractorStyle(),
			vtkCustomEvents::changeScrollValue,
			this, SLOT(changeScrollValue(vtkObject*, unsigned long, void*, void*)));
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetWidgets()
{
	if (m_vtkWidget)
	{
		m_vtkWidget.reset();
	}
	m_vtkWidget = std::make_unique<vtkWidget2D>();
	m_qtvtkWidget->SetRenderWindow(vtkNew<vtkGenericOpenGLRenderWindow>());
	m_renderWindow[0] = m_qtvtkWidget->GetRenderWindow();
	m_vtkWidget->setRenderWindow(m_renderWindow[0]);
	m_renderWindow[0]->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::resetScroll()
{
	if (m_scroll)
	{
		const QSignalBlocker blocker(m_scroll);
		m_scroll->setValue(0);
		m_scroll->setMaximum(0);
	}
	else
	{
		m_scroll = new QScrollBar(Qt::Vertical, this);
		setScrollStyle();
	}
}

void asclepios::gui::Widget2D::setScrollStyle() const
{
	m_scroll->hide();
	QFile file(scroll2DStyle);
	if (file.open(QFile::ReadOnly))
	{
		const QString styleSheet = QLatin1String(file.readAll());
		m_scroll->setStyleSheet(styleSheet);
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget2D::initImageReader(vtkWidget2D* t_vtkWidget2D, Widget2D* t_self)
{
	t_vtkWidget2D->initImageReader();
	emit t_self->imageReaderInitialized();
}

//-----------------------------------------------------------------------------
bool asclepios::gui::Widget2D::canScrollBeRefreshed(const int& t_patientIndex, const int& t_studyIndex,
                                                    const int& t_seriesIndex) const
{
	return m_vtkWidget && m_vtkWidget->getImage() && t_patientIndex == m_patientIndex &&
		t_studyIndex == m_studyIndex && m_seriesIndex == t_seriesIndex;
}
