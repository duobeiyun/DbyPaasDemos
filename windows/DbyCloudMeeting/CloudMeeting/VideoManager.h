#pragma once

#include <QWidget>
#include "ui_VideoManager.h"
#include <QStyledItemDelegate>
#include <QList>
#include <QStringList>
#include "QEventDef.h"
#include "VideoWidget.h"

class VideoManager : public QWidget
{
	Q_OBJECT

public:
	VideoManager(QWidget *parent = Q_NULLPTR);
	~VideoManager();

	void ReAddViewList(QString uid);	//从main窗口转到viewlist
	bool GetRemoteStreamState(QString uid, STRU_REMOTE_USER &info);
signals:
	void SigRemoteVideoShowMain(QString uid);
signals:
	void SigUpdateUserList(QString, bool);
signals:
	void SigUserOnlineState(QString uid, QString msg);
signals:
	void SigUserOfflineState(QString uid, QString msg);

public slots:
	void OnRemoteUserJoined(QString uid);
	void OnShowRemoteVideo(QString uid, const QByteArray& y, uint size, uint w, uint h);
	void OnRemoteUserOffline(QString uid);
	void OnVideoWidgetShowMain(QString);
	void OnVideoPlayerInit(QString);
	void OnVideoPlayerDestroy(QString);
	void OnAudioPlayerInit(QString);
	void OnAudioPlayerDestroy(QString);
	void OnVideoStreamMuteState(QString uid, bool mute);
	void OnAudioStreamMuteState(QString uid, bool mute);
	void OnSetRemoteVolume(QString userinfo, int vtotal);
protected:
	//void showEvent(QShowEvent *event);
private:
	void initWidget();
	void setVideoWidget(QString uid, const QByteArray& y, uint size, uint w, uint h);
private:
	int findUserInfoFromVec(QString uid);
	void eraseFromVec(QString uid);
	void setUserInfoFromVec(QString uid, REMOTEUSER_STATE_TYPE state);
	void setFirstFrameFromVec(QString uid, bool isFirstFrame);
	void addRemoteViewList(QString uid);
	bool DeleteFromVideoView(QString id);
	void addVec(QString uid, REMOTEUSER_STATE_TYPE state = USER_VALID_STATE);
	void setRemoteVideoStreamState(int index);
	void setRemoteAudioStreamState(int index);
	void setRemoteAudioVolume(QMap<QString, int> map);
	void setFirstFrameState(int index, bool state, int w, int h);
	VideoWidget *findVideoWgt(QString uid);
private:
	Ui::VideoManager ui;

	QVector<STRU_REMOTE_USER> m_RemoteUserVec;	//所有远端用户队列
	QStringList m_RemoteViewList;	//在远端视频列表显示的用户	
};