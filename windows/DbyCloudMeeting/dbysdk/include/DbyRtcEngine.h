#pragma once
#include <stdint.h>
#include <stddef.h>

#define DBYAPI_EXPORT

#define DBY_CALL __cdecl

#if defined(DBYAPI_EXPORT)
#define DBY_API extern "C" __declspec(dllexport)
#else
#define DBY_API extern "C" __declspec(dllimport)
#endif

class DbyRtcEngineEventHandler;

struct DbyRtcEngineContext
{
	DbyRtcEngineEventHandler* eventHandler;
	const char* appId;
	const char* appkey;
	void* context;
	DbyRtcEngineContext()
		:eventHandler(NULL)
		, appId(NULL)
		, appkey(NULL)
		, context(NULL)
	{}
};

enum RENDER_MODE_TYPE
{
	/**
  1: Uniformly scale the video until it fills the visible boundaries (cropped). One dimension of the video may have clipped contents.
   */
	RENDER_MODE_HIDDEN = 1,
	/**
2: Uniformly scale the video until one of its dimension fits the boundary (zoomed to fit). Areas that are not filled due to disparity in the aspect ratio are filled with black.
 */
	RENDER_MODE_FIT = 2,
	/** **DEPRECATED** 3: This mode is deprecated.
	 */
	RENDER_MODE_ADAPTIVE = 3,
};

struct VideoCanvas
{
	void* view;
	int renderMode;
	uint32_t uid;

	VideoCanvas()
		: view(NULL)
		, renderMode(RENDER_MODE_HIDDEN)
		, uid(0)
	{}
	VideoCanvas(void* v, int m, uint32_t u)
		: view(v)
		, renderMode(m)
		, uid(u)
	{}
};

class DbyRtcEngine
{
protected:
	virtual ~DbyRtcEngine() {}
public:
	virtual int initialize(const DbyRtcEngineContext& context) = 0;

	virtual int setChannelProfile(int profile) = 0;

	virtual int setUserRole(int role) = 0;

	virtual int joinChannel(const char* channelId, const char* uid, const char* nickname) = 0;
	virtual void leaveChannel() = 0;

	virtual int enableVideo() = 0;
	virtual int enableLocalVideo(bool enabled) = 0;
	virtual int enableLocalAudio(bool enabled) = 0;

	virtual void setVideoLoadType(int type) = 0; //0-hwnd; 1-self render
	virtual int setupLocalVideo(const VideoCanvas& canvas) = 0;
	virtual int setupRemoteVideo(const VideoCanvas& canvas) = 0;

	virtual void enableAudioVolumeIndication(int interval, bool report_local) = 0;

	virtual int muteRemoteVideoStream(const char* uid, bool mute) = 0;
	virtual int muteRemoteAudioStream(const char* uid, bool mute) = 0;

	virtual void setDualStreamMode(bool status) = 0;
	virtual bool setDualStream(const char* uid, int mode) = 0;
	virtual void setDualStreamToHighVideo(const char* uid) = 0;

	virtual int getCameraCount() = 0;
	virtual int getCamera(int index, char* deviceName, int len) = 0;
	virtual int setCamera(const char* deviceName) = 0;

	virtual int getMicrophoneCount() = 0;
	virtual int getMicrophone(int index, char* deviceName, int len) = 0;
	virtual int setMicrophone(const char* deviceName) = 0;

	virtual bool sendChatMessage(const char *textMessage) = 0;

	virtual const char* getApiVersion() = 0;

	virtual void setAppPack(const char* name) = 0;
	virtual void setAppVersion(const char* ver) = 0;
	virtual void setEnvironment(int env) = 0;
};

class DbyRtcEngineEventHandler
{
public:
	virtual ~DbyRtcEngineEventHandler() {}

	virtual void onJoinChannelSuccess(const char* channel, const char* uid) {
		(void)channel;
		(void)uid;
	}

	virtual void onRejoinChannelSuccess() {

	}

	virtual void onLeaveChannel(int code) {

	}

	virtual void onLocalVideoData(uint8_t *y, int w, int h) {
		(void)y;
		(void)w;
		(void)h;
	}

	virtual void onRemoteVideoData(const char* id, char* data, uint32_t size, int32_t width, int32_t height) {
		(void)id;
		(void)data;
		(void)size;
		(void)width;
		(void)height;
	}

	virtual void onLocalAudioStateChanged(int errortype, int errorcode) {
		(void)errortype;
		(void)errorcode;
	}

	virtual void onLocalVideoStateChanged(int errortype, int errorcode) {
		(void)errortype;
		(void)errorcode;
	}
	/*
		userinfo: {"12":34, "234":45}
	*/
	virtual void onAudioVolumeIndication(const char* userinfo, int totalvolume) {
		(void)userinfo;
		(void)totalvolume;
	}

	//state: 0-init,1-stop,2-first frame
	virtual void onRemoteVideoStateChanged(const char* uid, const char* deviceName, int state) {
		(void)uid;
		(void)deviceName;
		(void)state;
	}

	virtual void onRemoteMicStateChange(const char* uid, const char* deviceName, int state) {
		(void)uid;
		(void)deviceName;
		(void)state;
	}

	virtual void onUserJoined(const char* uid) {
		(void)uid;
	}

	virtual void onUserLeave(const char* uid) {
		(void)uid;
	}

	virtual void onClientKickOff(void) {

	}

	virtual void onBroadcastMessage(const char* uid, const char* msg) {
		(void)uid;
		(void)msg;
	}

	virtual void onNetworkDelay(int delay) {
		(void)delay;
	}

	virtual void onConnectionStateChanged(int code) {
		(void)code;
	}

	virtual void onClientRoleChanged(int oldRole, int newRole) {
		(void)oldRole;
		(void)newRole;
	}
};

DBY_API DbyRtcEngine*  DBY_CALL createDbyRtcEngine();
DBY_API void DBY_CALL destroyDbyRtcEngine();