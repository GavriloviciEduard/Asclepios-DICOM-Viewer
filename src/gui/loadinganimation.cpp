#include "loadinganimation.h"

asclepios::gui::LoadingAnimation::LoadingAnimation(QWidget* parent)
	: QDialog(parent)
{
	initView();
}

//-----------------------------------------------------------------------------
void asclepios::gui::LoadingAnimation::initView()
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint
		| Qt::WindowSystemMenuHint | Qt::Dialog);
	m_ui.label->setMovie(&m_movie);
	m_movie.setFileName(":/loading");
	m_movie.start();
}
