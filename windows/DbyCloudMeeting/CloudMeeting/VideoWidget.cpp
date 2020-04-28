#include "VideoWidget.h"
#include <QDebug>
VideoWidget::VideoWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initDlg();
}

VideoWidget::~VideoWidget()
{
	disconnect(this, SIGNAL(SigShowRemoteVideo(const QByteArray&, uint, uint)), ui.openGLWidget, SLOT(SlotShowYuv(const QByteArray&, uint, uint)));
	qDebug() << "~VideoWidget m_uid = " << m_uid;
}

//void VideoWidget::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QWidget::showEvent(event);
//}

void VideoWidget::SetUid(QString id)
{
	m_uid = id;
	ui.vuidlabel->setText(QString::fromLocal8Bit(" %1").arg(id));
	ui.openGLWidget->SetUid(id);
}

QString VideoWidget::GetUid()
{
	return m_uid;
}

void VideoWidget::SetVideoSize(int w, int h)
{
	if (w != m_w || h != m_h) {
		m_w = w;
		m_h = h;
		ui.sizeLabel->setText(QString("w:%1 h:%2").arg(w).arg(h));
	}
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit SigRemoteVideoShowMain(m_uid);
}

void VideoWidget::initDlg()
{
	m_bgImageWgt = new CameraImageWidget(ui.widget);
	m_bgImageWgt->setMinimumSize(QSize(200, 112));
	ui.gridLayout->addWidget(m_bgImageWgt, 0, 0, 0, 0);
	ui.openGLWidget->hide();
	ui.audioLabel->setText(QString::fromLocal8Bit("ÉùÒô£º¹Ø±Õ"));
	ui.volumeLabel->setText(QString::fromLocal8Bit("ÒôÁ¿£º0"));
	connect(this, SIGNAL(SigShowRemoteVideo(const QByteArray&, uint, uint)), ui.openGLWidget, SLOT(SlotShowYuv(const QByteArray&, uint, uint)));
}

void VideoWidget::SetVideoStreamState(bool video)
{
	if (!m_bgImageWgt)
		return;
	ui.openGLWidget->hide();
	m_bgImageWgt->show();
	if (video) {
		m_bgImageWgt->Show(true);
	}
	else {
		m_bgImageWgt->Show(false);
	}
}

void VideoWidget::SetAudioStreamState(bool audio)
{
	if (audio) {
		ui.audioLabel->setText(QString::fromLocal8Bit("ÉùÒô£º¿ªÆô"));
	}
	else {
		ui.audioLabel->setText(QString::fromLocal8Bit("ÉùÒô£º¹Ø±Õ"));
	}
}

void VideoWidget::SetAudioVolumeState(VOLUME_STATE state)
{
	m_volumeState = state;
}

void VideoWidget::SetAudioVolume(int volume)
{
	int ntemp = m_volumeState == VOLUME_CLOSE ? 0 : volume;
	ui.volumeLabel->setText(QString::fromLocal8Bit("ÒôÁ¿£º%1").arg(ntemp));
}

void VideoWidget::SetVideoFirstFrame()
{
	ui.openGLWidget->show();
	m_bgImageWgt->hide();
}