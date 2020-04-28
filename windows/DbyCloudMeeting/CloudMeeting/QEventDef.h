#pragma once
#include <QObject>
#include <QString>

#define VERSION "V 1.2.13"
#define VERSION_NUM "1.2.13"
#define MAXSIZE 512
#define CID "channelid"
#define UID "userid"
#define LOGIN "login"
#define ENVIRONMENT "environment"

enum REMOTEUSER_STATE_TYPE
{
	USER_VALID_STATE = -1,
	USER_JOIN_VIEWLIST_STATE,		//���������Ƶ�б��
	USER_JOIN_MAIN_STATE,		//��������ʾ
	USER_OFFLING_STATE		//׼������
};

enum BG_IMAGE_STATE
{
	BG_INIT,
	BG_LOADING,
	BG_CLOSE,
	BG_RENDING
};

enum LOCAL_SHOW_PLACE
{
	LOCAL_PLACE_MAIN,
	LOCAL_PLACE_MIN
};

enum VOLUME_STATE {
	VOLUME_OPEN = -2,
	VOLUME_CLOSE = -1
};

enum CLOSE_EVENT_STATE {
	CLOSE_EVENT_STATE_INIT = -1,
	CLOSE_EVENT_STATE_CLOSING,
	CLOSE_EVENT_STATE_OTHERS
};


typedef struct _STRU_REMOTE_USER
{
	QString uid = "";
	REMOTEUSER_STATE_TYPE state = USER_VALID_STATE;
	bool isaudioplayeropen = false;
	bool isvideoplayeropen = false;
	bool isvideomuteopen = true;
	bool isaudiomuteopen = true;
	bool isFirstFrame = false; //��һ֡��
	VOLUME_STATE volumestate = VOLUME_CLOSE;
}STRU_REMOTE_USER, *P_STRU_REMOTE_USER;

typedef struct _STRU_LOCAL_USER
{
	QString uid;
	bool isaudioopen = false;
	bool isvideoopen = false;
	int showplace = LOCAL_PLACE_MAIN;			//0-main; 1-minimain 
} STRU_LOCAL_USER;

namespace duobei {

	constexpr int APPS_CONNECT_OK = 10;       //apps ���������ӳɹ�
	constexpr int APPS_CONNECT_ERR = 11;      //apps ���Ӵ���
	constexpr int APPS_RECONNECT = 12;        //apps ����
	constexpr int APPS_NO_CLIENTONLINE = 13;  //APPS_NO_CLIENTONLINE
	constexpr int APPS_DISCONNECT = 14;       // �Ͽ�
	constexpr int APPS_CONNECT_FATAL = 15;    // ����ǳ����

	constexpr int AUDIO_CONNECT_OK = 100;   //�������ӳɹ�
	constexpr int AUDIO_CONNECT_ERR = 101;  //��������ʧ��
	constexpr int AUDIO_RECONNECT = 102;    //��������

	constexpr int AUDIO_DISCONNECT = 104;

	constexpr int VIDEO_CONNECT_OK = 200;   //��Ƶ���ӳɹ�
	constexpr int VIDEO_CONNECT_ERR = 201;  //��Ƶ����ʧ��
	constexpr int VIDEO_RECONNECT = 202;    //��Ƶ����

	constexpr int VIDEO_DISCONNECT = 204;

	constexpr int GETAUTHINFO_BEGIN = 300;
	constexpr int GETAUTHINFO_OK = 301;
	constexpr int GETAUTHINFO_ERROR = 302;

	constexpr int TESTVM_BEGIN = 400;
	constexpr int TESTVM_OK = 401;
	constexpr int TESTVM_ERROR = 402;

	constexpr int STOP_ROOM_LOG = 500;

	constexpr int SOL_MESSAGE = 600;
	constexpr int SOL_MESSAGE_P2P = 601;
	constexpr int SOL_MESSAGE_P2P_FLOW = 602;

	//1δ��ʼ��2���ڽ��У�3���������ۿ��طţ�4û���˲μ�ֱ����5�ط�ת��ʧ�ܣ�6�ط�ת����
	//////////////����״̬
	constexpr int ALL_OK = 0;
	constexpr int AUTH_ANALYSIS_FAILED = 900;  //authinfo ����ʧ��
	constexpr int AUTH_HTTP_ERR = 901;
	constexpr int AUTH_TESTVM_EMPTY = 902;
	constexpr int AUTH_TESTVM_TIMEOUT = 903;
	constexpr int AUTH_RETURN_FALSE = 10000;  //����ʧ��
	constexpr int AUTH_NOT_BEGIN = 10001;
	constexpr int AUTH_CAN_WATCH_PLAYBACK = 10003;
	constexpr int AUTH_NO_ONE_TO_LIVE = 10004;
	constexpr int AUTH_PLAYBACK_CONVERSION_FAILED = 10005;
	constexpr int AUTH_PLAYBACK_CONVERSION = 10006;
	constexpr int VM_TESTVM_ERROR = 10007;
	constexpr int APPS_STARTAPPS_ERROR = 10008;
	constexpr int AUTH_LIVE_END = 10012;


	constexpr int PB_OK = 100000;
	constexpr int PB_HTTP_ERR = 100001;
	constexpr int PB_LOCALPATH_ERR = 100002;
	constexpr int PB_JSON_ERR = 100003;
	constexpr int PB_MODE_ERR = 100004;
	constexpr int PB_AUTH_ERR = 100006;
	constexpr int PB_OPENFILE_ERR = 100007;
	constexpr int PB_FLVFILE_ERR = 100008;
	constexpr int PB_READFILE_ERR = 100009;
	constexpr int PB_LOADING_FILE = 100010;
	constexpr int PB_LOADING_FILE_OK = 100011;
	constexpr int PB_DUPLICATE_LOGIN = 100012;

	constexpr int PB_SEEK_ERROR = 100014;
	constexpr int PB_READ_TIMEOUT = 100015;
	constexpr int PB_READ_END = 100016;

	constexpr int PLAYBACK_DATA_OPEN_FAIL = 100017;
	constexpr int PLAYBACK_DATA_EMPTY_CONTENT = 100018;
	constexpr int PLAYBACK_HTTP_FILE_GETSIZE_ERROR = 100019;
	constexpr int PLAYBACK_HTTP_FILE_DOWNLOAD_ERROR = 100020;
	constexpr int PLAYBACK_HTTP_FILE_DOWNLOAD_SIZE_ERROR = 100021;

	constexpr int NET_ERROR = 200000;
	constexpr int NET_OK = 200001;

	constexpr int kAudioEncodeError = 2001;
	constexpr int kAudioDecodeError = 2002;
	constexpr int kVideoEncodeError = 2003;
	constexpr int kVideoDecodeError = 2004;
}  // namespace duobei
