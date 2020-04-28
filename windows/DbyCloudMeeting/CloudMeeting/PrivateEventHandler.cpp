#include "PrivateEventHandler.h"
#include <QDebug>
#include <thread>
void yuvwrite(const char* name, uint8_t* y, int w, int h);
PrivateEventHandler::PrivateEventHandler()
{

}

PrivateEventHandler::~PrivateEventHandler()
{
}

void PrivateEventHandler::onJoinChannelSuccess(const char* channel, const char* uid)
{
	qDebug() << "onJoinChannelSuccess";
}

void PrivateEventHandler::onRejoinChannelSuccess()
{
	qDebug() << "onRejoinChannelSuccess";
}

void PrivateEventHandler::onLeaveChannel(int code)
{
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigLeaveChannelDestroy", Q_ARG(int, code));
}

void PrivateEventHandler::onLocalVideoData(uint8_t *y, int w, int h)
{
	std::thread::id tid = std::this_thread::get_id();
	

	ObjectManager* pMngrObj = ObjectManager::GetObjectManager();

	QString uid = pMngrObj->GetLocalUid();

	uint size = w * h * 3 / 2;
	QByteArray arr((char*)y, size);
	
	int res = QMetaObject::invokeMethod(pMngrObj, "SigProcessLocalYuv",Qt::QueuedConnection, Q_ARG(const QByteArray&, arr), Q_ARG(uint, w), Q_ARG(uint, h));
}

void PrivateEventHandler::onRemoteVideoData(const char* id, char* data, uint32_t size, int32_t width, int32_t height)
{
	std::thread::id tid = std::this_thread::get_id();
	ObjectManager* pMngrObj = ObjectManager::GetObjectManager();
	
	QString uid(id);
	QByteArray arr(data, size);
	
	int res = QMetaObject::invokeMethod(pMngrObj, "SigShowRemoteVideo", Qt::DirectConnection, Q_ARG(QString, uid), Q_ARG(const QByteArray&, arr), Q_ARG(uint, size), Q_ARG(uint, width), Q_ARG(uint, height));
}

void PrivateEventHandler::onLocalAudioStateChanged(int errortype, int errorcode)
{
	qDebug() << "onLocalAudioStateChanged errortype = " << errortype << " errorcode = " << errorcode;
}

void PrivateEventHandler::onLocalVideoStateChanged(int errortype, int errorcode)
{
	qDebug() << "onLocalVideoStateChanged errortype = " << errortype << " errorcode = " << errorcode;
}

void PrivateEventHandler::onAudioVolumeIndication(const char* userinfo, int totalvolume)
{
	QString info(userinfo);
	if (info.contains("null"))
		return;
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigSetRemoteVolume", Qt::QueuedConnection, Q_ARG(QString, info), Q_ARG(int, totalvolume));
}

void PrivateEventHandler::onRemoteVideoStateChanged(const char* uid, const char* deviceName, int state)
{
	QString struid(uid);
	qDebug() << "onRemoteVideoStateChanged id = " << struid << " state = " << state;
	if (state == 0) {
		int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigVideoPlayerInit", Qt::QueuedConnection, Q_ARG(QString, struid));
	}
	else if (state == 1) {
		int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigVideoPlayerDestroy", Qt::QueuedConnection, Q_ARG(QString, struid));
	}
}

void PrivateEventHandler::onRemoteMicStateChange(const char* uid, const char* deviceName, int state)
{
	QString struid(uid);
	qDebug() << "onRemoteMicStateChange id = " << struid << " state = " << state;
	if (state == 0) {
		int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigAudioPlayerInit", Qt::QueuedConnection, Q_ARG(QString, struid));
	}
	else if (state == 1) {
		int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigAudioPlayerDestroy", Qt::QueuedConnection, Q_ARG(QString, struid));
	}
}

void PrivateEventHandler::onUserJoined(const char* uid)
{
	QString userid(uid);

	qDebug() << "onClientOnline -id = " << QString(uid);

	ObjectManager* pMngrObj = ObjectManager::GetObjectManager();
	int res = QMetaObject::invokeMethod(pMngrObj, "SigRemoteUserJoined", Qt::QueuedConnection, Q_ARG(QString, userid));
}

void PrivateEventHandler::onUserLeave(const char* uid)
{
	QString userid(uid);

	qDebug() << "onClientOffline id = " << QString(uid);

	ObjectManager* pMngrObj = ObjectManager::GetObjectManager();

	int res = QMetaObject::invokeMethod(pMngrObj, "SigRemoteUserOffline", Qt::QueuedConnection, Q_ARG(QString, userid));
}

void PrivateEventHandler::onBroadcastMessage(const char* uid, const char* msg)
{
	QString struid = QString::fromLocal8Bit(uid);
	if (ObjectManager::GetObjectManager()->GetLocalUid() == struid)
		return;
	QString strmsg = QString::fromStdString(msg);
	ObjectManager* pMngrObj = ObjectManager::GetObjectManager();
	int res = QMetaObject::invokeMethod(pMngrObj, "SigRemoteUserChatMsg", Qt::QueuedConnection, Q_ARG(QString, struid), Q_ARG(QString, strmsg));
}

void PrivateEventHandler::onClientKickOff(void)
{
	qDebug() << QString::fromLocal8Bit("下线");
	ObjectManager* pMngrObj = ObjectManager::GetObjectManager();
	int res = QMetaObject::invokeMethod(pMngrObj, "SigLeaveChannel", Qt::DirectConnection);
}

void PrivateEventHandler::onNetworkDelay(int delay)
{
	int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigUpdateNetworkDelay", Q_ARG(int, delay));
}

void PrivateEventHandler::onConnectionStateChanged(int code)
{
	QString msgInfo;

	switch (code)
	{
	case duobei::APPS_CONNECT_OK:
	{
		msgInfo = QString::fromLocal8Bit("服务器连接成功");
		break;
	}
	case duobei::APPS_CONNECT_ERR:
	{
		msgInfo = QString::fromLocal8Bit("连接错误");
		break;
	}
	case duobei::APPS_RECONNECT:
	{
		msgInfo = QString::fromLocal8Bit("正在重连");
		break;
	}
	case duobei::APPS_DISCONNECT:
	{
		msgInfo = QString::fromLocal8Bit("断开连接");
		break;
	}
	case duobei::APPS_CONNECT_FATAL:
	{
		msgInfo = QString::fromLocal8Bit("网络很差");
		break;
	}
	default:
		return;
	}

	int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigUpdateStatusInformation", Q_ARG(int, code), Q_ARG(QString, msgInfo));
}

void PrivateEventHandler::onClientRoleChanged(int oldRole, int newRole)
{
	int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigClientRoleChanged", Q_ARG(int, oldRole), Q_ARG(int, newRole));
}

void yuvwrite(const char* name, uint8_t* y, int w, int h)
{
	std::string strname  = name;
	strname += ".yuv";
	FILE *pFile = fopen(strname.c_str(), "ab+");
	if (!pFile)
		return;
	uint8_t *u = y + w * h;
	uint8_t *v = u + w * h / 4;
	fwrite(y, 1, w * h, pFile);
	fwrite(u, 1, w * h / 4, pFile);
	fwrite(v, 1, w * h / 4, pFile);
	fclose(pFile);
}