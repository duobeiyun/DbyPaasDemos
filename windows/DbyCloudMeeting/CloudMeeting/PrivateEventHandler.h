#pragma once
#include <DbyRtcEngine.h>
#include "ObjectManager.h"
class PrivateEventHandler :
	public DbyRtcEngineEventHandler
{
public:
	PrivateEventHandler();
	virtual ~PrivateEventHandler();

	virtual void onJoinChannelSuccess(const char* channel, const char* uid) override;
	virtual void onRejoinChannelSuccess() override;
	virtual void onLeaveChannel(int code) override;

	virtual void onLocalVideoData(uint8_t *y, int w, int h) override;
	virtual void onRemoteVideoData(const char* id, const char* devicename, char* data, uint32_t size, int32_t width, int32_t height) override;

	virtual void onLocalAudioStateChanged(int errortype, int errorcode) override;
	virtual void onLocalVideoStateChanged(int errortype, int errorcode) override;

	virtual void onAudioVolumeIndication(const char* userinfo, int totalvolume) override;

	virtual void onRemoteVideoStateChanged(const char* uid, const char* deviceName, int state) override;
	virtual void onRemoteMicStateChange(const char* uid, const char* deviceName, int state) override;

	virtual void onUserJoined(const char* uid) override;
	virtual void onUserLeave(const char* uid) override;

	virtual void onBroadcastMessage(const char* uid, const char* msg) override;
	virtual void onClientKickOff(void) override;
	virtual void onNetworkDelay(int delay) override;
	virtual void onConnectionStateChanged(int code) override;
	virtual void onClientRoleChanged(int oldRole, int newRole) override;
private:

};

