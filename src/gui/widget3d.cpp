#include "widget3d.h"
#include "tabwidget.h"
#include <QFocusEvent>
#include <vtkRendererCollection.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QKeyEvent>
#include <QtConcurrent/qtconcurrentrun.h>

asclepios::gui::Widget3D::Widget3D(QWidget* parent)
	: WidgetBase(parent)
{
	initData();
	initView();
	createConnections();
	m_tabWidget = parent;
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::render()
{
	if (!m_image)
	{
		throw std::runtime_error("No image for widget 3d!");
	}
	try
	{
		m_toolbar->getUI().toolButtonCrop->setVisible(false);
		m_toolbar->getUI().comboBoxFilters->setVisible(false);
		startLoadingAnimation();
		m_vtkWidget->setImage(m_image);
		m_vtkWidget->setSeries(m_series);
		m_vtkWidget->setInteractor(m_qtvtkWidget->GetInteractor());
		m_future = QtConcurrent::run(onRenderAsync, this);
		Q_UNUSED(connect(this, &Widget3D::finishedRenderAsync,
			this, &Widget3D::onFinishedRenderAsync));
	}
	catch (const std::exception& ex)
	{
		//todo log
	}
}

//-----------------------------------------------------------------------------
bool asclepios::gui::Widget3D::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		auto* const keyEvent = dynamic_cast<QKeyEvent*>(event);
		const int key = keyEvent->key();
		auto* const combo = m_toolbar->getUI().comboBoxFilters;
		switch (key)
		{
		case Qt::Key_Left:
			{
				const int currentIndex = combo->currentIndex();
				combo->setCurrentIndex(!combo->currentIndex()
					                       ? combo->count() - 1
					                       : currentIndex - 1);
				break;
			}
		case Qt::Key_Right:
			{
				const int currentIndex = combo->currentIndex();
				combo->setCurrentIndex(currentIndex == combo->count() - 1
					                       ? 0
					                       : currentIndex + 1);
				break;
			}
		default:
			break;
		}
	}
	return QWidget::eventFilter(watched, event);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::onfilterChanged(const QString& t_filter) const
{
	if (m_qtvtkWidget && m_vtkWidget)
	{
		m_vtkWidget->setFilter(t_filter);
		m_qtvtkWidget->GetRenderWindow()->Render();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::onCropPressed(const bool& t_pressed) const
{
	m_vtkWidget->activateBoxWidget(t_pressed);
	m_qtvtkWidget->GetRenderWindow()->Render();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::onActivateWidget(const bool& t_flag)
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
void asclepios::gui::Widget3D::onSetMaximized() const
{
	if (m_tabWidget)
	{
		dynamic_cast<TabWidget*>
			(m_tabWidget)->onMaximize();
	}
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::onFinishedRenderAsync()
{
	auto* const renderWindow =
		m_qtvtkWidget->GetRenderWindow();
	renderWindow->AddRenderer(m_vtkWidget->
		getRenderWindows()[0]->GetRenderers()->
		                       GetFirstRenderer());
	renderWindow->Render();
	onfilterChanged(m_toolbar->getUI()
	                         .comboBoxFilters->itemData(0).toString());
	stopLoadingAnimation();
	disconnect(this, &Widget3D::finishedRenderAsync,
	           this, &Widget3D::onFinishedRenderAsync);
	m_toolbar->getUI().toolButtonCrop->setVisible(true);
	m_toolbar->getUI().comboBoxFilters->setVisible(true);
	installEventFilter(this);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::onRenderAsync(Widget3D* t_self)
{
	t_self->m_vtkWidget->render();
	emit t_self->finishedRenderAsync();
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::initView()
{
	m_ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	auto* const layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setMargin(0);
	layout->addWidget(m_toolbar);
	layout->addWidget(m_qtvtkWidget);
	setLayout(layout);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::initData()
{
	m_qtvtkWidget = new QVTKOpenGLNativeWidget(this);
	m_qtvtkWidget->SetRenderWindow(vtkNew<vtkGenericOpenGLRenderWindow>());
	m_qtvtkWidget->GetRenderWindow()->SetDoubleBuffer(true);
	m_vtkWidget = std::make_unique<vtkWidget3D>();
	m_toolbar = new ToolbarWidget3D(this);
	m_vtkEvents = std::make_unique<vtkEventFilter>(this);
	setWidgetType(WidgetType::widget3d);
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::createConnections()
{
	if (m_toolbar)
	{
		Q_UNUSED(connect(m_toolbar,
			&ToolbarWidget3D::filterChanged, this,
			&Widget3D::onfilterChanged));
		Q_UNUSED(connect(m_toolbar, &ToolbarWidget3D::cropPressed,
			this, &Widget3D::onCropPressed));
	}
	setFocusPolicy(Qt::FocusPolicy::WheelFocus);
	m_qtvtkWidget->installEventFilter(m_vtkEvents.get());
	Q_UNUSED(connect(m_vtkEvents.get(),
		&vtkEventFilter::activateWidget,
		this, &Widget3D::onActivateWidget));
	Q_UNUSED(connect(m_vtkEvents.get(),
		&vtkEventFilter::setMaximized,
		this, &Widget3D::onSetMaximized));
}

//-----------------------------------------------------------------------------
void asclepios::gui::Widget3D::startLoadingAnimation()
{
	m_loadingAnimation = std::make_unique<LoadingAnimation>(this);
	m_loadingAnimation->setWindowFlags(Qt::Widget);
	layout()->addWidget(m_loadingAnimation.get());
	m_loadingAnimation->show();
}
