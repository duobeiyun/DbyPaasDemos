#include "VideoManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "VideoWidget.h"
#include "ObjectManager.h"
#include "NoFocusDelegate.h"

VideoManager::VideoManager(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initWidget();

	ObjectManager *pObjMnger = ObjectManager::GetObjectManager();

	connect(this, SIGNAL(SigUpdateUserList(QString, bool)), pObjMnger, SIGNAL(SigUpdateUserList(QString, bool)));
}

VideoManager::~VideoManager()
{
}

//void VideoManager::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QWidget::showEvent(event);
//}

void VideoManager::initWidget()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0.7)");
	/*ui.tableWidget->verticalHeader()->setDefaultSectionSize(150);*/

	ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.tableWidget->setFrameShape(QTableWidget::NoFrame);

	ui.tableWidget->verticalHeader()->setHidden(true);
	ui.tableWidget->horizontalHeader()->setHidden(true);
	ui.tableWidget->setColumnCount(1);
	ui.tableWidget->setColumnWidth(0, this->width());
	ui.tableWidget->setItemDelegate(new NoFocusDelegate());
	ui.tableWidget->setStyleSheet("selection-background-color:white;");
}

void VideoManager::OnShowRemoteVideo(QString uid, const QByteArray& y, uint size, uint w, uint h)
{
	//qDebug() << "OnShowRemoteVideo: " << uid;
	int index = findUserInfoFromVec(uid);
	if (index < 0)
		return;

	if (m_RemoteUserVec.at(index).state == USER_JOIN_MAIN_STATE) {
		ObjectManager* pMngrObj = ObjectManager::GetObjectManager();
		QMetaObject::invokeMethod(pMngrObj, "SigProcessShowMain", Q_ARG(const QByteArray&, y), Q_ARG(uint, w), Q_ARG(uint, h));
	}
	else {
		setVideoWidget(uid, y, size, w, h);
	}

	setFirstFrameState(index, true, w, h);
}

void VideoManager::OnRemoteUserJoined(QString uid)
{
	QString strLocalUid = ObjectManager::GetObjectManager()->GetLocalUid();
	if (uid == strLocalUid)
		return;

	if (findUserInfoFromVec(uid) >= 0)
		return;

	if (m_RemoteUserVec.count() == 0) {
		addVec(uid);
		emit SigRemoteVideoShowMain(uid);
		setUserInfoFromVec(uid, USER_JOIN_MAIN_STATE);
	}
	else {
		addVec(uid);
		setUserInfoFromVec(uid, USER_JOIN_VIEWLIST_STATE);
		addRemoteViewList(uid);
	}

	emit SigUpdateUserList(uid, true);
	emit SigUserOnlineState(uid, QString::fromLocal8Bit(("已上线")));
}

void VideoManager::OnRemoteUserOffline(QString uid)
{

	QString strLocalUid = ObjectManager::GetObjectManager()->GetLocalUid();
	if (uid == strLocalUid)
		return;

	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	if (m_RemoteUserVec.at(index).state == USER_JOIN_MAIN_STATE)
	{
		if (m_RemoteViewList.count() <= 0)
		{
			QString localuid = ObjectManager::GetObjectManager()->GetLocalUid();
			emit SigRemoteVideoShowMain(localuid);
		}
		else
		{
			VideoWidget* pItem = (VideoWidget*)ui.tableWidget->cellWidget(0, 0);
			if (pItem && pItem->metaObject()->className() == QString("VideoWidget"))
			{
				QString struid = QString("%1").arg(pItem->GetUid());
				emit SigRemoteVideoShowMain(struid);
				setUserInfoFromVec(struid, USER_JOIN_MAIN_STATE);
				DeleteFromVideoView(struid);
			}
		}
	}

	setUserInfoFromVec(uid, USER_OFFLING_STATE);

	DeleteFromVideoView(uid);

	emit SigUpdateUserList(uid, false);
	emit SigUserOnlineState(uid, QString::fromLocal8Bit("已下线"));
}


bool VideoManager::DeleteFromVideoView(QString id)
{
	int index = findUserInfoFromVec(id);
	if (index < 0)
		return false;

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		VideoWidget* pW = qobject_cast<VideoWidget*>(ui.tableWidget->cellWidget(i, 0));
		QString name = pW->metaObject()->className();
		if (name != "VideoWidget")	continue;
		if (pW->GetUid() == id)
		{
			ui.tableWidget->removeRow(i);
			m_RemoteViewList.removeOne(id);
			break;
		}
	}

	if (m_RemoteUserVec.at(index).state == USER_OFFLING_STATE)
		m_RemoteUserVec.removeAt(index);

	return true;
}

void VideoManager::setVideoWidget(QString uid, const QByteArray& y, uint size, uint w, uint h)
{
	//查找对应的wgt
	VideoWidget* pItem = findVideoWgt(uid);
	if (pItem) {
		pItem->SetVideoSize(w, h);
		int res = QMetaObject::invokeMethod(pItem, "SigShowRemoteVideo", Q_ARG(const QByteArray&, y), Q_ARG(uint, w), Q_ARG(uint, h));
	}
}

int VideoManager::findUserInfoFromVec(QString uid)
{
	int flag = -1;
	for (int i = 0; i < m_RemoteUserVec.count(); i++)
	{
		if (m_RemoteUserVec.at(i).uid == uid) {
			flag = i;
			break;
		}
	}

	return flag;
}

void VideoManager::eraseFromVec(QString uid)
{
	for (auto Iter = m_RemoteUserVec.begin(); Iter != m_RemoteUserVec.end(); Iter++)
	{
		if (Iter->uid == uid) {
			m_RemoteUserVec.erase(Iter);
			break;
		}
	}
}

void VideoManager::addVec(QString uid, REMOTEUSER_STATE_TYPE state)
{
	STRU_REMOTE_USER info;
	info.uid = uid;
	info.state = state;
	m_RemoteUserVec.push_back(info);
}

void VideoManager::setUserInfoFromVec(QString uid, REMOTEUSER_STATE_TYPE state)
{
	for (auto iter = m_RemoteUserVec.begin(); iter != m_RemoteUserVec.end(); iter++)
	{
		if (iter->uid == uid) {
			iter->state = state;
			break;
		}
	}
}

void VideoManager::setFirstFrameFromVec(QString uid, bool isFirstFrame)
{
	for (auto iter = m_RemoteUserVec.begin(); iter != m_RemoteUserVec.end(); iter++)
	{
		if (iter->uid == uid) {
			iter->isFirstFrame = isFirstFrame;
			break;
		}
	}
}

void VideoManager::addRemoteViewList(QString uid)
{
	m_RemoteViewList.append(uid);

	int iRowCount = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(iRowCount);

	VideoWidget *wgt = new VideoWidget(ui.tableWidget);

	connect(wgt, SIGNAL(SigRemoteVideoShowMain(QString)), this, SLOT(OnVideoWidgetShowMain(QString)));
	ui.tableWidget->setCellWidget(iRowCount, 0, wgt);
	ui.tableWidget->setRowHeight(iRowCount, wgt->height());
	ui.tableWidget->selectRow(iRowCount);

	//设置volume state
	int index = findUserInfoFromVec(uid);
	if (index >= 0) {
		wgt->SetAudioVolumeState(m_RemoteUserVec[index].volumestate);
	}

	wgt->SetUid(uid);
}

void VideoManager::ReAddViewList(QString uid)
{
	int index = findUserInfoFromVec(uid);
	if (index >= 0) {
		setUserInfoFromVec(uid, USER_JOIN_VIEWLIST_STATE);
		addRemoteViewList(uid);
		//
		setRemoteVideoStreamState(index);
		setRemoteAudioStreamState(index);
		//第一帧初始化
		m_RemoteUserVec[index].isFirstFrame = false;
	}
}

void VideoManager::OnVideoWidgetShowMain(QString uid)
{
	setUserInfoFromVec(uid, USER_JOIN_MAIN_STATE);
	DeleteFromVideoView(uid);

	//first frame init
	int index = findUserInfoFromVec(uid);
	if (index >= 0) {
		m_RemoteUserVec[index].isFirstFrame = false;
	}

	emit SigRemoteVideoShowMain(uid);

	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigSetMainVolumeState", Q_ARG(QString, uid), Q_ARG(int, m_RemoteUserVec[index].volumestate));
}

void VideoManager::OnVideoPlayerInit(QString uid)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	/*if (m_RemoteUserVec[index].isvideoplayeropen || )
		return;*/

	m_RemoteUserVec[index].isvideoplayeropen = true;

	//first frame init
	m_RemoteUserVec[index].isFirstFrame = false;
	
	setRemoteVideoStreamState(index);
}

void VideoManager::OnVideoPlayerDestroy(QString uid)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	//first frame init
	m_RemoteUserVec[index].isFirstFrame = false;

	/*if (!m_RemoteUserVec[index].isvideoopen)
		return;*/

	//first frame init
	m_RemoteUserVec[index].isvideoplayeropen = false;


	setRemoteVideoStreamState(index);
}

void VideoManager::OnAudioPlayerInit(QString uid)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	m_RemoteUserVec[index].isaudioplayeropen = true;


	setRemoteAudioStreamState(index);

	m_RemoteUserVec[index].volumestate = VOLUME_OPEN;
	if (m_RemoteUserVec[index].state == USER_JOIN_VIEWLIST_STATE) {
		VideoWidget* pW = findVideoWgt(uid);
		if (pW)
			pW->SetAudioVolumeState(VOLUME_OPEN);
	}
	else if (m_RemoteUserVec[index].state == USER_JOIN_MAIN_STATE) {
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigSetMainVolumeState", Q_ARG(QString, uid), Q_ARG(int, VOLUME_OPEN));
	}
}

void VideoManager::OnAudioPlayerDestroy(QString uid)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	m_RemoteUserVec[index].isaudioplayeropen = false;

	setRemoteAudioStreamState(index);


	//设置音量状态
	m_RemoteUserVec[index].volumestate = VOLUME_CLOSE;
	if (m_RemoteUserVec[index].state == USER_JOIN_VIEWLIST_STATE) {
		VideoWidget* pW = findVideoWgt(uid);
		if (pW)
			pW->SetAudioVolumeState(VOLUME_CLOSE);
	}
	else if (m_RemoteUserVec[index].state == USER_JOIN_MAIN_STATE) {
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigSetMainVolumeState", Q_ARG(QString, uid), Q_ARG(int, VOLUME_CLOSE));
	}
}

void VideoManager::OnVideoStreamMuteState(QString uid, bool mute)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	/*if (m_RemoteUserVec[index].isvideoopen)
		return;*/

	m_RemoteUserVec[index].isvideomuteopen = mute;

	m_RemoteUserVec[index].isFirstFrame = false;

	setRemoteVideoStreamState(index);
}

void VideoManager::OnAudioStreamMuteState(QString uid, bool mute)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return;

	m_RemoteUserVec[index].isaudiomuteopen = mute;

	setRemoteAudioStreamState(index);
}

void VideoManager::OnSetRemoteVolume(QString userinfo, int vtotal)
{
	// 解析json
	QMap<QString, int> map;
	map.clear();

	QJsonParseError parseJsonErr;
	QJsonDocument document = QJsonDocument::fromJson(userinfo.toUtf8(), &parseJsonErr);
	if (!(parseJsonErr.error == QJsonParseError::NoError))
	{
		qDebug() << QString::fromLocal8Bit("解析json文件错误！");
		qDebug() << "userinfo : " << userinfo;
		return;
	}
	QJsonObject jsonObject = document.object();
	QStringList uidlist = jsonObject.keys();
	for (auto i = 0; i < uidlist.count(); i++)
	{
		map[uidlist[i]] = jsonObject.value(uidlist[i]).toVariant().toInt();
	}
	
	setRemoteAudioVolume(map);
}

void VideoManager::setRemoteVideoStreamState(int index)
{
	if (m_RemoteUserVec[index].state == USER_JOIN_VIEWLIST_STATE) {
		
		VideoWidget* pW = findVideoWgt(m_RemoteUserVec[index].uid);
		if(pW)
			pW->SetVideoStreamState(m_RemoteUserVec[index].isvideomuteopen && m_RemoteUserVec[index].isvideoplayeropen);
	}
	else if (m_RemoteUserVec[index].state == USER_JOIN_MAIN_STATE) {
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeRemoteStreamState", Q_ARG(QString, m_RemoteUserVec[index].uid));	
	}
}

void VideoManager::setRemoteAudioStreamState(int index)
{
	if (m_RemoteUserVec[index].state == USER_JOIN_VIEWLIST_STATE) {

		VideoWidget* pW = findVideoWgt(m_RemoteUserVec[index].uid);
		if (pW)
			pW->SetAudioStreamState(m_RemoteUserVec[index].isaudiomuteopen && m_RemoteUserVec[index].isaudioplayeropen);
	}
	else if (m_RemoteUserVec[index].state == USER_JOIN_MAIN_STATE) {
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeRemoteStreamState", Q_ARG(QString, m_RemoteUserVec[index].uid));
	}
}

void VideoManager::setRemoteAudioVolume(QMap<QString, int> map)
{
	for (auto iter = map.begin(); iter != map.end(); iter++)
	{
		int index = findUserInfoFromVec(iter.key());
		if (index == -1)
			continue;
		if (m_RemoteUserVec[index].state == USER_JOIN_VIEWLIST_STATE) {
			VideoWidget* pW = findVideoWgt(iter.key());
			if (pW)
				pW->SetAudioVolume(iter.value());
		}
		else if (m_RemoteUserVec[index].state == USER_JOIN_MAIN_STATE) {
			QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigSetRemoteAudioVolumeOnMain", Q_ARG(QString, iter.key()), Q_ARG(int, iter.value()));
		}
	}
}

void VideoManager::setFirstFrameState(int index, bool state, int w, int h)
{
	if (m_RemoteUserVec[index].isFirstFrame == state)
		return;

	qDebug() << "uid = " << m_RemoteUserVec[index].uid << " w = " << w << " h = " << h;

	m_RemoteUserVec[index].isFirstFrame = state;
	//setFirstFrameFromVec(m_RemoteUserVec[index].uid, state);
	if (!state)
		return;
	
	REMOTEUSER_STATE_TYPE statetemp = m_RemoteUserVec[index].state;

	if (statetemp == USER_JOIN_VIEWLIST_STATE) {
		VideoWidget* p = findVideoWgt(m_RemoteUserVec[index].uid);
		if (p && m_RemoteUserVec[index].isvideomuteopen && m_RemoteUserVec[index].isvideoplayeropen) {
			p->SetVideoFirstFrame();
		}
	}
	else if (statetemp == USER_JOIN_MAIN_STATE) {
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeRemoteStreamState", Q_ARG(QString, m_RemoteUserVec[index].uid));
	}
}

bool VideoManager::GetRemoteStreamState(QString uid, STRU_REMOTE_USER &info)
{
	int index = findUserInfoFromVec(uid);
	if (index == -1)
		return false;

	info.uid = m_RemoteUserVec[index].uid;

	info.state = m_RemoteUserVec[index].state;
	
	info.isaudioplayeropen = m_RemoteUserVec[index].isaudioplayeropen;
	info.isvideoplayeropen = m_RemoteUserVec[index].isvideoplayeropen;

	info.isaudiomuteopen = m_RemoteUserVec[index].isaudiomuteopen;
	info.isvideomuteopen = m_RemoteUserVec[index].isvideomuteopen;

	info.isFirstFrame = m_RemoteUserVec[index].isFirstFrame;
	return true;
}

VideoWidget *VideoManager::findVideoWgt(QString uid)
{	
	VideoWidget* pW = nullptr;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		pW = qobject_cast<VideoWidget*>(ui.tableWidget->cellWidget(i, 0));
		QString name = pW->metaObject()->className();
		if (name != "VideoWidget")	continue;
		if (pW->GetUid() == uid)
			break;
		else
			pW = nullptr;
	}
	return pW;
}