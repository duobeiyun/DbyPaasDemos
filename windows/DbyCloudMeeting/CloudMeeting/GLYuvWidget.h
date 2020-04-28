#pragma once

// MyOpenGLWidget.h

#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "YUV420P_Render.h"
#include "QEventDef.h"
struct FrameData {
	uint8_t *y;
	uint8_t *u;
	uint8_t *v;
	int y_linesize;
	int u_linesize;
	int v_linesize;
	int width;
	int height;
};

class GLYuvWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit GLYuvWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	void setFrame(FrameData *frame);
	void SetUid(QString uid);
	virtual ~GLYuvWidget();
public slots:
	void SlotShowYuv(const QByteArray& ptr, uint width, uint height);
	void SlotSetGLState(QString uid, int state);
protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	//void showEvent(QShowEvent *event) override;
private:
	
private:
	YUV420P_Render *pRender = NULL;
	QByteArray m_ba;
	uint m_w = 0;
	uint m_h = 0;
	QString m_uid;
	BG_IMAGE_STATE m_bgState = BG_INIT;
};

#endif // MYOPENGLWIDGET_H