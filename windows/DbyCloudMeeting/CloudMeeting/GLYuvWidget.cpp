#include "GLYuvWidget.h"
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QPainter>
#include <thread>
#include "ObjectManager.h"

#define VERTEXIN 0
#define TEXTUREIN 1


GLYuvWidget::GLYuvWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
	pRender = new YUV420P_Render;
	this->setStyleSheet("background-color:transparent");
}

GLYuvWidget::~GLYuvWidget()
{
	if (pRender)
	{
		delete pRender;
		pRender = nullptr;
	}

	qDebug() << "~GLYuvWidget m_uid = " << m_uid;
}

void GLYuvWidget::initializeGL()
{
	pRender->initialize();
}

void GLYuvWidget::setFrame(FrameData *frame)
{
	/*if (this->pCurrentFrame != NULL) {
		delete this->pCurrentFrame;
	}
	this->pCurrentFrame = frame;

	if (pCurrentFrame == NULL) {
		return;
	}

	update();*/
}

void GLYuvWidget::paintGL()
{
	std::thread::id tid = std::this_thread::get_id();

	int x, y, w, h;
	float fPer = (float)width() / (float)height();
	if (fPer > 1.f)
	{
		h = height();
		w = (float)h / (float)m_h * (float)m_w;
		x = (width() - w) / 2;
		y = 0;
	}
	else
	{
		w = width();
		h = (float)width()*(float)m_h / (float)m_w;
		x = 0;
		y = (height() - h) / 2;
	}

	pRender->setGLView(x, y, w, h);

	/*if (m_bgState == BG_INIT)
	{
		pRender->setBackgroundImage();
		qDebug() << "¶ÔÏó = " << this->objectName();
		qDebug() << "paintGL uid = " << m_uid;
		return;
	}*/
	
	
	if (m_ba.size() == 0 || m_w == 0 || m_h == 0)
		return;

	pRender->render(m_ba, m_w, m_h, 0);
}


void GLYuvWidget::SlotShowYuv(const QByteArray& ptr, uint width, uint height)
{
	if (ptr.size() == 0 || width == 0 || height == 0)
		return;
	m_ba = ptr;
	m_w = width;
	m_h = height;
	update();
}

void GLYuvWidget::resizeGL(int w, int h)
{
	update();
}

void GLYuvWidget::SetUid(QString uid)
{
	m_uid = uid;
}

//void GLYuvWidget::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QOpenGLWidget::showEvent(event);
//}

void GLYuvWidget::SlotSetGLState(QString uid, int state)
{
	if (m_uid != uid)
		return;

	qDebug() << "m_uid == " << m_uid;

	m_bgState = (BG_IMAGE_STATE)state;
}