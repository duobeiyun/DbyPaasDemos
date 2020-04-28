#pragma once

#include <QWidget>
#include "ui_CameraImageWidget.h"
#include <QMovie>
#include <QPixmap>

class CameraImageWidget : public QWidget
{
	Q_OBJECT

public:
	CameraImageWidget(QWidget *parent = Q_NULLPTR);
	~CameraImageWidget();

	void SetLabelSize(int w = 138);

	void Show(bool loading = true);
	
private:
	Ui::CameraImageWidget ui;
	QMovie *m_pMovie = nullptr;
};
