#pragma once

#include <QMainWindow>
#include "ui_DbMainWindow.h"
#include "CameraImageWidget.h"
#include "ControllerWidget.h"
class DbMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	DbMainWindow(QWidget *parent = Q_NULLPTR);
	~DbMainWindow();


public:
	void JoinChannel(QString channelId, QString userId, QString nickname, bool isDualStream, bool profile, bool role);
signals:
	void SigLeaveChannel();
public slots:
	void OnProcessShowMain(const QByteArray& ba, uint w, uint h);
	void OnSetMainUid(QString uid);
	void OnLeaveChannel();
	void OnChangeLocalStreamState();
	void OnChangeRemoteStreamState(QString);
	void OnQuitLoading();
protected:
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
private:
	void initDlg();
private:
	Ui::DbMainWindow ui;
	ControllerWidget* m_Controller = nullptr;
	CameraImageWidget *m_pBgWgt = nullptr;
	CLOSE_EVENT_STATE m_nQuitState = CLOSE_EVENT_STATE_INIT;
	bool m_isQuitLoading = false;
};
