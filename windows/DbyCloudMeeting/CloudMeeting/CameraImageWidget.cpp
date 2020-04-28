#include "CameraImageWidget.h"
#include <QImage>
#include <QDebug>

CameraImageWidget::CameraImageWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background:rgb(28, 28, 28)");
	SetLabelSize(69);

	m_pMovie = new QMovie("./Resources/loading.gif");
}

CameraImageWidget::~CameraImageWidget()
{
	if (m_pMovie) {
		m_pMovie->stop();
		m_pMovie->deleteLater();
		m_pMovie = nullptr;
		qDebug() << "m_pMovie destroyed";
	}
}

void CameraImageWidget::SetLabelSize(int w)
{
	int h = w * 153 / 138;
	QString strFileName = "./Resources/icon-camera-close-138-153.f7fb0f93.png";
	ui.label->setPixmap(QPixmap::fromImage(QImage(strFileName).scaled(w, h, Qt::KeepAspectRatio)));
}

void CameraImageWidget::Show(bool loading)
{
	ui.label->clear();
	if (loading) {
		ui.label->setMovie(m_pMovie);
		m_pMovie->start();
	}
	else {
		SetLabelSize(69);
	}
}
