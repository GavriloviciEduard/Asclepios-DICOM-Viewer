#include "loadinganimation.h"
#include <QMovie>
#include <QLabel>

asclepios::gui::LoadingAnimation::LoadingAnimation(QWidget *parent)
	: QWidget(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::LoadingAnimation::initView()
{
	m_ui.setupUi(this);
	hide();
}

//-----------------------------------------------------------------------------
void asclepios::gui::LoadingAnimation::start()
{
	QMovie* movie = new QMovie(":/loading");
	m_ui.label->setMovie(movie);
	movie->start();
	show();
}
