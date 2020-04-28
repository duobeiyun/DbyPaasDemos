#include "ObjectManager.h"

ObjectManager *ObjectManager::m_objMngr = nullptr;

ObjectManager::ObjectManager(QObject *parent)
	: QObject(parent)
{
}

ObjectManager::~ObjectManager()
{
}

ObjectManager *ObjectManager::GetObjectManager()
{
	if (m_objMngr == nullptr)
		m_objMngr = new ObjectManager(NULL);

	return m_objMngr;
}

void ObjectManager::SetVideoObj(VideoManager* obj)
{
	m_videoObj = obj;

	connect(m_objMngr, SIGNAL(SigRemoteUserJoined(QString)), m_videoObj, SLOT(OnRemoteUserJoined(QString)));
	connect(m_objMngr, SIGNAL(SigShowRemoteVideo(QString, const QByteArray &, uint, uint, uint)), m_videoObj, SLOT(OnShowRemoteVideo(QString, const QByteArray &, uint, uint, uint)));
	connect(m_objMngr, SIGNAL(SigRemoteUserOffline(QString)), m_videoObj, SLOT(OnRemoteUserOffline(QString)));
	connect(m_videoObj, SIGNAL(SigUserOnlineState(QString, QString)), m_objMngr, SIGNAL(SigUserOnlineState(QString, QString)));
	connect(m_videoObj, SIGNAL(SigUserOfflineState(QString, QString)), m_objMngr, SIGNAL(SigUserOfflineState(QString, QString)));

	connect(m_objMngr, SIGNAL(SigVideoPlayerInit(QString)), m_videoObj, SLOT(OnVideoPlayerInit(QString)));
	connect(m_objMngr, SIGNAL(SigVideoPlayerDestroy(QString)), m_videoObj, SLOT(OnVideoPlayerDestroy(QString)));
	connect(m_objMngr, SIGNAL(SigAudioPlayerInit(QString)), m_videoObj, SLOT(OnAudioPlayerInit(QString)));
	connect(m_objMngr, SIGNAL(SigAudioPlayerDestroy(QString)), m_videoObj, SLOT(OnAudioPlayerDestroy(QString)));

	connect(m_objMngr, SIGNAL(SigVideoStreamMuteState(QString, bool)), m_videoObj, SLOT(OnVideoStreamMuteState(QString, bool)));
	connect(m_objMngr, SIGNAL(SigAudioStreamMuteState(QString, bool)), m_videoObj, SLOT(OnAudioStreamMuteState(QString, bool)));
	connect(m_objMngr, SIGNAL(SigSetRemoteVolume(QString, int)), m_videoObj, SLOT(OnSetRemoteVolume(QString, int)));
}

VideoManager* ObjectManager::GetVideoObj()
{
	return m_videoObj;
}
void ObjectManager::SetLocalUid(QString uid)
{
	m_localUserState.uid = uid;
}
QString ObjectManager::GetLocalUid()
{
	return m_localUserState.uid;
}

void ObjectManager::SetLocalVideoStreamState(bool state)
{
	m_localUserState.isvideoopen = state;
}

void ObjectManager::SetLocalAudioStreamState(bool state)
{
	m_localUserState.isaudioopen = state;
}

void ObjectManager::SetLocalShowPlace(int place)
{
	m_localUserState.showplace = place;
}

bool ObjectManager::GetLocalVideoStreamState()
{
	return m_localUserState.isvideoopen;
}

bool ObjectManager::GetLocalAudioStreamState()
{
	return m_localUserState.isaudioopen;
}

int ObjectManager::GetLocalShowPlace()
{
	return m_localUserState.showplace;
}

void ObjectManager::ReleaseObjectManaget()
{
	if (m_objMngr)
	{
		delete m_objMngr;
		m_objMngr = nullptr;
	}
}