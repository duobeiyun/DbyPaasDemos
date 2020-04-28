#pragma once

#include <QWidget>
#include "ui_ControllerWidget.h"
#include "CameraImageWidget.h"
#include "ProcessState.h"
#include "NetSpeed.h"

class ControllerWidget : public QWidget
{
	Q_OBJECT

public:
	ControllerWidget(QWidget *parent = Q_NULLPTR);
	~ControllerWidget();

	QString GetMainUid();
	void SetMinLocalShowState(bool video);
	bool GetRemoteStreamState(QString uid, STRU_REMOTE_USER &info);
	void SetLocalMainAudioState(bool audio);
signals:
	void SigLeaveChannel();
	void SigShowMain(const QByteArray&, uint, uint);
	void SigSetMainUid(QString);
	void SigQuitLoading();
public slots:
	void OnJoinChannel(QString channelId, QString userId, QString nickname, bool isDualStream, bool profile, bool role);
	void OnRemoteVideoShowMain(QString uid);
	void OnVideoListCtrl();
	void OnChatCtrl();
	void OnUserListCtrl();
	void OnLeaveChannel();
	void OnLeaveChannelDestroy(int);
	void OnProcessLocalYuv(const QByteArray& ba, uint w, uint h);
	void OnProcessShowMain(const QByteArray& ba, uint w, uint h);
	void OnUpdateSysState(int cpu, int mem, QString tm);
	void OnUpdateNetworkDelay(int delay);
	void OnNetSpeed(QString netspeed);
	void OnSetRemoteAudioVolumeOnMain(QString, int);
	void OnClientRoleChanged(int, int);
	void OnChangeRole();
	void OnSetMainVolumeState(QString, int state);
protected:
	//void showEvent(QShowEvent *event) override;
private:
	void init();
	void changeLocalStreamShow();
	void setAnyDlgVisible(bool enable);	//Ö±²¥
private:
	Ui::ControllerWidget ui;

	CameraImageWidget *m_bgImageWgt = nullptr;

	QString m_localUid;
	QString m_mainUid;
	ProcessState *m_pProcessState = nullptr;
	NetSpeed *m_pNetSpeed = nullptr;
	bool m_isRemoteShowMain = false;
	int m_w = 0;
	int m_h = 0;

	VOLUME_STATE m_volumeState = VOLUME_CLOSE;

	int m_nQuitCode = -1;
};
