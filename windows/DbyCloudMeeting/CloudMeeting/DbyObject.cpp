#include "DbyObject.h"
#include "DbSettings.h"
#define INT_FAILED_RETURN(p) if(p == nullptr) return -1;
#define VOID_FAILED_RETURN(p) if(p == nullptr) return;
#define BOOL_FAILED_RETURN(p) if(p == nullptr) return false;

DbyObject* DbyObject::m_pDbyObject = nullptr;
DbyObject::DbyObject()
{
}

DbyObject::~DbyObject()
{
}

DbyObject* DbyObject::GetDbyObject()
{
	if (!m_pDbyObject)
	{
		m_pDbyObject = new DbyObject;
	}

	return m_pDbyObject;
}

DbyRtcEngine* DbyObject::GetDbyRtcEngine()
{
	if (!m_pDbyRtcEngine)
	{
		m_pDbyRtcEngine = createDbyRtcEngine();

		
		DbyRtcEngineContext ctx;
		ctx.appId = APPID;
		ctx.appkey = APPKEY;
		ctx.eventHandler = &m_Handler;
		m_pDbyRtcEngine->initialize(ctx);
	}

	return m_pDbyRtcEngine;
}

void DbyObject::Destroy()
{
	if (m_pDbyRtcEngine)
	{
		//createDbyRtcEngine();
		destroyDbyRtcEngine();
		m_pDbyRtcEngine = nullptr;
	}
}

int DbyObject::EnableVideo()
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->enableVideo();
}

int DbyObject::EnableLocalVideo(bool enabled)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->enableLocalVideo(enabled);
}

int DbyObject::EnableLocalAudio(bool enabled)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->enableLocalAudio(enabled);
}

int DbyObject::SetChannelProfile(int profile)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->setChannelProfile(profile);
}

int DbyObject::SetClientRole(int role)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->setUserRole(role);
}

void DbyObject::JoinChannel(std::string channelId, std::string uid, std::string nickname)
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->joinChannel(channelId.c_str(), uid.c_str(), nickname.c_str());
}

void DbyObject::LeaveChannel()
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->leaveChannel();
}

int DbyObject::MuteRemoteVideoStream(const char* uid, bool mute)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->muteRemoteVideoStream(uid,"", mute);
}

int DbyObject::MuteRemoteAudioStream(const char* uid, bool mute)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->muteRemoteAudioStream(uid, "", mute);
}

void DbyObject::SetDualStreamMode(bool status)
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->setDualStreamMode(status);
}

bool DbyObject::SetDualStream(const char* uid, int mode)
{
	BOOL_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->setDualStream(uid, mode);
}

void DbyObject::setDualStreamToHighVideo(const char* uid)
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->setDualStreamToHighVideo(uid);
}

int DbyObject::GetCameraCount()
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->getCameraCount();
}

int DbyObject::GetCamera(int index, char* deviceName, int len)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->getCamera(index, deviceName, len);
}

int DbyObject::SetCamera(const char* deviceName)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->setCamera(deviceName);
}

int DbyObject::GetMicrophoneCount()
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->getMicrophoneCount();
}

int DbyObject::GetMicrophone(int index, char* deviceName, int len)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->getMicrophone(index, deviceName, len);
}

int DbyObject::SetMicrophone(const char* deviceName)
{
	INT_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->setMicrophone(deviceName);
}

bool DbyObject::SendChatMsg(const char *textMessage)
{
	BOOL_FAILED_RETURN(m_pDbyRtcEngine)
	return m_pDbyRtcEngine->sendChatMessage(textMessage);
}

void DbyObject::EnableAudioVolumeIndication(int interval, bool report_local)
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->enableAudioVolumeIndication(interval, report_local);
}

void DbyObject::SetAppPack(const char* name)
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->setAppPack(name);
}

void DbyObject::SetAppVersion(const char* ver)
{
	VOID_FAILED_RETURN(m_pDbyRtcEngine)
	m_pDbyRtcEngine->setAppVersion(ver);
}