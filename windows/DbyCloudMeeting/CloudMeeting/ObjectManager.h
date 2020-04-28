#pragma once

#include <QObject>
#include <QMetaObject>
#include <QMutex>
#include "VideoManager.h"
#include "QEventDef.h"
class ObjectManager : public QObject
{
	Q_OBJECT
public:
	static ObjectManager *GetObjectManager();
	~ObjectManager();

	void SetVideoObj(VideoManager* obj);

	void SetLocalUid(QString uid);
	void SetLocalVideoStreamState(bool state);
	void SetLocalAudioStreamState(bool state);
	void SetLocalShowPlace(int place);

	QString GetLocalUid();
	bool GetLocalVideoStreamState();
	bool GetLocalAudioStreamState();
	int GetLocalShowPlace();
	VideoManager* GetVideoObj();

	static void ReleaseObjectManaget();

signals:
	void SigProcessLocalYuv(const QByteArray& ba, uint w, uint h);
signals:
	void SigProcessShowMain(const QByteArray& ba, uint w, uint h);
signals:
	void SigRemoteUserJoined(QString uid);
signals:
	void SigRemoteUserOffline(QString uid);
signals:
	void SigShowRemoteVideo(QString uid, const QByteArray& y, uint size, uint w, uint h);
signals:
	void SigUpdateUserList(QString, bool);
signals:
	void SigUserOnlineState(QString uid, QString msg);
signals:
	void SigUserOfflineState(QString uid, QString msg);
signals:
	void SigRemoteUserChatMsg(QString uid, QString msg);
signals:
	void SigLeaveChannel();

signals:
	void SigVideoPlayerInit(QString);
signals:
	void SigVideoPlayerDestroy(QString);
signals:
	void SigAudioPlayerInit(QString);
signals:
	void SigAudioPlayerDestroy(QString);
signals:
	void SigVideoStreamMuteState(QString, bool);
signals:
	void SigAudioStreamMuteState(QString, bool);
signals:
	void SigChangeRemoteStreamState(QString);
signals:
	void SigChangeLocalStreamState();
signals:
	void SigUpdateNetworkDelay(int delay);
signals:
	void SigUpdateStatusInformation(int, QString);
signals:
	void SigSetRemoteVolume(QString, int);
signals:
	void SigSetRemoteAudioVolumeOnMain(QString, int);
signals:
	void SigSetMainVolumeState(QString, int);
signals:
	void SigClientRoleChanged(int, int);
signals:
	void SigLeaveChannelDestroy(int);
private:
	ObjectManager(QObject *parent);
private:
	static ObjectManager* m_objMngr;
	VideoManager* m_videoObj = nullptr;
	STRU_LOCAL_USER m_localUserState;
};