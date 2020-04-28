#pragma once
#include <iostream>
#include "PrivateEventHandler.h"

class DbyObject
{
public:
	DbyObject();
	~DbyObject();

	int EnableVideo();
	int EnableLocalVideo(bool enabled);
	int EnableLocalAudio(bool enabled);

	int SetChannelProfile(int profile);
	int SetClientRole(int role);

	int JoinChannel(std::string channelId, std::string uid, std::string nickname);
	void LeaveChannel();

	int MuteRemoteVideoStream(const char* uid, bool mute);
	int MuteRemoteAudioStream(const char* uid, bool mute);


	void SetDualStreamMode(bool status);
	bool SetDualStream(const char* uid, int mode);
	void setDualStreamToHighVideo(const char* uid);

	int GetCameraCount();
	int GetCamera(int index, char* deviceName, int len);
	int SetCamera(const char* deviceName);

	int GetMicrophoneCount();
	int GetMicrophone(int index, char* deviceName, int len);
	int SetMicrophone(const char* deviceName);

	bool SendChatMsg(const char *textMessage);

	void EnableAudioVolumeIndication(int interval, bool report_local);

	void SetAppPack(const char* name);
	void SetAppVersion(const char* ver);

	DbyRtcEngine* GetDbyRtcEngine();
	void Destroy();
public:
	static DbyObject* GetDbyObject();
public:
	DbyRtcEngine* m_pDbyRtcEngine = nullptr;
	PrivateEventHandler m_Handler;

	static DbyObject* m_pDbyObject;
};

