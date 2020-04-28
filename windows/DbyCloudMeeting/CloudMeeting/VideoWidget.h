#pragma once

#include <QWidget>
#include "ui_VideoWidget.h"
#include "CameraImageWidget.h"
class VideoWidget : public QWidget
{
	Q_OBJECT

public:
	VideoWidget(QWidget *parent = Q_NULLPTR);
	~VideoWidget();

	void SetUid(QString id);
	QString GetUid();
	void SetVideoSize(int w, int h);

	void SetVideoStreamState(bool video);
	void SetAudioStreamState(bool audio);
	void SetAudioVolume(int volume);
	void SetAudioVolumeState(VOLUME_STATE);
	void SetVideoFirstFrame();
protected:
	//void showEvent(QShowEvent *event);
signals:
	void SigShowRemoteVideo(const QByteArray& y, uint w, uint h);
signals:
	void SigRemoteVideoShowMain(QString);
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
private:
	void initDlg();
private:
	Ui::VideoWidget ui;
	QString m_uid;
	int m_w = 0;
	int m_h = 0;
	VOLUME_STATE m_volumeState = VOLUME_CLOSE;
	CameraImageWidget *m_bgImageWgt = nullptr;
};
