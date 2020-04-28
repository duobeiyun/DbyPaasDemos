#include "ControllerWidget.h"
#include <QDebug>
#include "ObjectManager.h"
#include "DbyObject.h"

ControllerWidget::ControllerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

ControllerWidget::~ControllerWidget()
{
	qDebug() << "ControllerWidget::~ControllerWidget()";
}

void ControllerWidget::OnJoinChannel(QString channelId, QString userId, QString nickname, bool isDualStream, bool profile, bool role)
{
	/*this->showMaximized();*/

	//ui.mainManager->SetUid(userId);

	if (profile) {
		if(!role)
			setAnyDlgVisible(false);
	}
	else {
		ui.pushButton_role->hide();
	}

	DbyObject *pDbyObj = DbyObject::GetDbyObject();
	DbyRtcEngine* pRtcEngine = pDbyObj->GetDbyRtcEngine();
	ObjectManager *objMngr = ObjectManager::GetObjectManager();
	objMngr->SetLocalUid(userId);
	objMngr->SetVideoObj(ui.videoManager);
	pDbyObj->SetAppPack(QString::fromLocal8Bit("多贝云会议（PC）").toStdString().c_str());
	pDbyObj->SetAppVersion(VERSION_NUM);
	pDbyObj->SetDualStreamMode(isDualStream);

	if (!(profile && !role)) {
		if (pDbyObj->EnableVideo() == -1) {
			qDebug() << "no camera ";
		}
		else {
			objMngr->SetLocalVideoStreamState(true);
			objMngr->SetLocalAudioStreamState(true);
		}
	}
	
	pDbyObj->EnableAudioVolumeIndication(200, true);

	pDbyObj->SetChannelProfile(profile ? 1 : 0);
	pDbyObj->SetClientRole(role ? 1 : 2);

	int ret = pDbyObj->JoinChannel(channelId.toStdString(), userId.toStdString(), nickname.toStdString());

	if (ret != 0) {
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigUpdateStatusInformation", Q_ARG(int, ret), Q_ARG(QString, QString::fromLocal8Bit("加入频道失败！")));
		qDebug() << QString("JoinChannel failed!!! ret = %1").arg(ret);
	}

	UserListWidget *userListWidget = (UserListWidget *)ui.stackedWidget->widget(0);
	userListWidget->AddLocalUid(userId);

	ui.toolbarWidget->Init();

	m_localUid = userId;
	m_mainUid = userId;
	QString text = QString::fromLocal8Bit("用户：");
	text += userId;
	ui.mainUidLabel->setText(text);

	objMngr->SetLocalShowPlace(LOCAL_PLACE_MAIN);
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");

	m_pProcessState->start();
	m_pNetSpeed->start();

}

void ControllerWidget::OnRemoteVideoShowMain(QString uid)
{
	if (m_mainUid != uid && m_localUid != uid) {
		m_isRemoteShowMain = true;
		ui.videoManager->ReAddViewList(m_mainUid);
		DbyObject::GetDbyObject()->SetDualStream(uid.toStdString().c_str(), 1);
		DbyObject::GetDbyObject()->SetDualStream(m_mainUid.toStdString().c_str(), 0);
		//ui.mainManager->SetUid(uid);
		emit SigSetMainUid(uid);
		ui.minLocalWidget->SetUid(m_localUid);

		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeRemoteStreamState", Q_ARG(QString, uid));
	}

	if (uid == m_localUid) {
		m_isRemoteShowMain = false;
	}

	m_mainUid = uid;
	QString text = QString::fromLocal8Bit("用户：");
	text += uid;
	ui.mainUidLabel->setText(text);
	qDebug() << "mainuidlabel uid = " << ui.mainUidLabel->text();

	
	changeLocalStreamShow();
}

void ControllerWidget::OnVideoListCtrl()
{
	static bool bEnable = false;
	ui.videoManager->isHidden() ? ui.videoManager->show() : ui.videoManager->hide();
}

void ControllerWidget::OnChatCtrl()
{
	if (ui.stackedWidget->isHidden() || ui.stackedWidget->currentIndex() != 1) {
		ui.stackedWidget->show();
		ui.stackedWidget->setCurrentIndex(1);
	}
	else {
		ui.stackedWidget->hide();
	}
}

void ControllerWidget::OnUserListCtrl()
{
	if (ui.stackedWidget->isHidden() || ui.stackedWidget->currentIndex() != 0) {
		ui.stackedWidget->setCurrentIndex(0);
		ui.stackedWidget->show();
	}
	else {
		ui.stackedWidget->hide();
	}
}

void ControllerWidget::OnLeaveChannel()
{	
	if (m_nQuitCode == 0)
		return;

	emit SigQuitLoading();
	qDebug() << "OnLeaveChannel-----start" << m_nQuitCode;
	DbyObject::GetDbyObject()->LeaveChannel();
	m_nQuitCode = 0;
}


void ControllerWidget::OnLeaveChannelDestroy(int code) 
{
	DbyObject::GetDbyObject()->Destroy();
	qDebug() << "OnLeaveChannel-----code = "<<code;

	if (m_pProcessState)
	{
		m_pProcessState->Stop();
		m_pProcessState->deleteLater();
		m_pProcessState = nullptr;
	}

	if (m_pNetSpeed)
	{
		m_pNetSpeed->Stop();
		m_pNetSpeed->deleteLater();
		m_pNetSpeed = nullptr;
	}

	//QObject* pSender = sender();

	/*if (pSender)*/ {
		emit SigLeaveChannel();
	}

	//ObjectManager::ReleaseObjectManaget();

	this->close();
}


void ControllerWidget::init()
{
	/*
	this->setWindowIcon(QIcon("./Resources/duobei.ico"));
	this->setWindowTitle(QString::fromLocal8Bit("多贝云会议"));*/
	ui.miniwidget->setStyleSheet("background:rgb(28, 28, 28)");
	ui.audioLabel->setText(QString::fromLocal8Bit("声音：关闭"));
	ui.volumeLabel->setText(QString::fromLocal8Bit("音量：0"));
	m_bgImageWgt = new CameraImageWidget(ui.miniwidget);
	m_bgImageWgt->setObjectName("mainShowCameraPic");
	ui.gridLayout->addWidget(m_bgImageWgt, 0, 1, 1, 1);
	ui.minLocalWidget->hide();
	m_bgImageWgt->hide();
	m_pProcessState = new ProcessState;
	m_pNetSpeed = new NetSpeed;

	ui.stackedWidget->setCurrentIndex(0);

	ObjectManager *pMngerObj = ObjectManager::GetObjectManager();
	connect(pMngerObj, SIGNAL(SigProcessLocalYuv(const QByteArray&, uint, uint)), this, SLOT(OnProcessLocalYuv(const QByteArray&, uint, uint)));
	connect(pMngerObj, SIGNAL(SigProcessShowMain(const QByteArray&, uint, uint)), this, SLOT(OnProcessShowMain(const QByteArray&, uint, uint)));
	connect(pMngerObj, SIGNAL(SigLeaveChannel()), this, SLOT(OnLeaveChannel()));
	connect(pMngerObj, SIGNAL(SigUpdateNetworkDelay(int)), this, SLOT(OnUpdateNetworkDelay(int)));
	connect(pMngerObj, SIGNAL(SigSetMainVolumeState(QString, int)), this, SLOT(OnSetMainVolumeState(QString, int)));
	connect(pMngerObj, SIGNAL(SigSetRemoteAudioVolumeOnMain(QString, int)), this, SLOT(OnSetRemoteAudioVolumeOnMain(QString, int)));
	connect(pMngerObj, SIGNAL(SigClientRoleChanged(int, int)), this, SLOT(OnClientRoleChanged(int, int)));
	connect(pMngerObj, SIGNAL(SigLeaveChannelDestroy(int)), this, SLOT(OnLeaveChannelDestroy(int)));
	

	connect(ui.pushButton_role, SIGNAL(clicked()), this, SLOT(OnChangeRole()));

	

	connect(ui.videoManager, SIGNAL(SigRemoteVideoShowMain(QString)), this, SLOT(OnRemoteVideoShowMain(QString)));
	connect(ui.toolbarWidget, SIGNAL(SigLeaveChannel()), this, SLOT(OnLeaveChannel()));
	connect(ui.toolbarWidget, SIGNAL(SigVideoListCtrl()), this, SLOT(OnVideoListCtrl()));
	connect(ui.toolbarWidget, SIGNAL(SigChatCtrl()), this, SLOT(OnChatCtrl()));
	connect(ui.toolbarWidget, SIGNAL(SigUserListCtrl()), this, SLOT(OnUserListCtrl()));

	connect(m_pProcessState, SIGNAL(SigCpuMemUsageRate(int, int, QString)), this, SLOT(OnUpdateSysState(int, int, QString)));
	connect(m_pNetSpeed, SIGNAL(SigNetSpeed(QString)), this, SLOT(OnNetSpeed(QString)));
	
}

void ControllerWidget::OnProcessLocalYuv(const QByteArray& ba, uint w, uint h)
{
	if (m_isRemoteShowMain) {
		int res = QMetaObject::invokeMethod(ui.minLocalWidget, "SlotShowYuv", Qt::QueuedConnection, Q_ARG(const QByteArray&, ba), Q_ARG(uint, w), Q_ARG(uint, h));
	}
	else {
		if (m_h != h || m_w != w) {
			ui.mainsizeLabel->setText(QString("w: %1 h: %2 ").arg(w).arg(h));
		}
		emit SigShowMain(ba, w, h);
	}
}

void ControllerWidget::OnProcessShowMain(const QByteArray& ba, uint w, uint h)
{
	if (m_isRemoteShowMain) {
		if (m_h != h || m_w != w) {
			ui.mainsizeLabel->setText(QString("w: %1 h: %2 ").arg(w).arg(h));
		}
		emit SigShowMain(ba, w, h);
	}
}

void ControllerWidget::OnUpdateSysState(int cpu, int mem, QString tm)
{
	QString strCpu = QString::fromLocal8Bit("cpu：");
	strCpu += QString("%1%").arg(cpu);
	QString strMem = QString::fromLocal8Bit("内存：");
	strMem += QString("%1M").arg(mem);
	QString strTm = QString::fromLocal8Bit("时长：");
	strTm += tm;
	ui.cpuLabel->setText(strCpu);
	ui.memLabel->setText(strMem);

	ui.durationLabel->setText(strTm);
}

void ControllerWidget::OnUpdateNetworkDelay(int delay)
{
	ui.delayLabel->setText(QString::fromLocal8Bit("时延：%1ms").arg(delay));
}

void ControllerWidget::OnNetSpeed(QString netspeed)
{
	ui.netspeedLabel->setText(netspeed);
}

//void ControllerWidget::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QMainWindow::showEvent(event);
//}

QString ControllerWidget::GetMainUid()
{
	return m_mainUid;
}

void ControllerWidget::SetMinLocalShowState(bool video)
{
	if (video) {
		ui.minLocalWidget->show();
		m_bgImageWgt->hide();
	}
	else {
		ui.minLocalWidget->hide();
		m_bgImageWgt->show();
		m_bgImageWgt->Show(false);
	}
}

void ControllerWidget::changeLocalStreamShow()
{
	if (m_isRemoteShowMain) {

		//mini main
		if (ObjectManager::GetObjectManager()->GetLocalShowPlace() == 1)
			return;

		ObjectManager::GetObjectManager()->SetLocalShowPlace(LOCAL_PLACE_MIN);
		bool isvideo = ObjectManager::GetObjectManager()->GetLocalVideoStreamState();
		SetMinLocalShowState(isvideo);
	}
	else {
		//main 
		if (ObjectManager::GetObjectManager()->GetLocalShowPlace() == 0)
			return;


		ObjectManager::GetObjectManager()->SetLocalShowPlace(LOCAL_PLACE_MAIN);
		ui.minLocalWidget->hide();
		m_bgImageWgt->hide();

		bool isaudio = ObjectManager::GetObjectManager()->GetLocalAudioStreamState();
		ui.audioLabel->setText(isaudio ? QString::fromLocal8Bit("声音：开启") : QString::fromLocal8Bit("声音：关闭"));
		ui.volumeLabel->setText("");
		QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");
	}
}

bool ControllerWidget::GetRemoteStreamState(QString uid, STRU_REMOTE_USER &info)
{
	if (ui.videoManager == nullptr)
		return false;

	bool ret = ui.videoManager->GetRemoteStreamState(uid, info);

	if (info.isaudiomuteopen && info.isaudioplayeropen) {
		ui.audioLabel->setText(QString::fromLocal8Bit("声音：开启"));
	}
	else {
		ui.audioLabel->setText(QString::fromLocal8Bit("声音：关闭"));
	}
	
	return ret;
}

void ControllerWidget::SetLocalMainAudioState(bool audio)
{
	if (audio) {
		ui.audioLabel->setText(QString::fromLocal8Bit("声音：开启"));
	}
	else {
		ui.audioLabel->setText(QString::fromLocal8Bit("声音：关闭"));
	}
}

void ControllerWidget::OnSetMainVolumeState(QString uid, int state)
{
	if (m_mainUid != uid)
		return;

	m_volumeState = (VOLUME_STATE)state;
}

void ControllerWidget::OnSetRemoteAudioVolumeOnMain(QString uid, int volume)
{
	if (m_mainUid != uid)
		return;

	int ntemp = m_volumeState == VOLUME_CLOSE ? 0 : volume;

	ui.volumeLabel->setText(QString::fromLocal8Bit("音量：%1").arg(ntemp));
}

void ControllerWidget::OnClientRoleChanged(int oldrole, int newrole)
{
	QString strRole;
	if (newrole == 1) {
		strRole = QString::fromLocal8Bit("主播");
		setAnyDlgVisible(true);
		//DbyObject::GetDbyObject()->EnableLocalVideo(true);
		//DbyObject::GetDbyObject()->EnableLocalAudio(true);
		ObjectManager::GetObjectManager()->SetLocalAudioStreamState(true);
		ObjectManager::GetObjectManager()->SetLocalVideoStreamState(true);

		
	}
	else if (newrole == 2) {
		strRole = QString::fromLocal8Bit("观众");
		setAnyDlgVisible(false);
		//DbyObject::GetDbyObject()->EnableLocalVideo(false);
		//DbyObject::GetDbyObject()->EnableLocalAudio(false);
		ObjectManager::GetObjectManager()->SetLocalAudioStreamState(false);
		ObjectManager::GetObjectManager()->SetLocalVideoStreamState(false);
	}

	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");
	ui.pushButton_role->setText(strRole);
}

void ControllerWidget::OnChangeRole()
{
	QString str = ui.pushButton_role->text();

	int role = str == QString::fromLocal8Bit("主播") ? 2 : 1;

	DbyObject::GetDbyObject()->SetClientRole(role);
}

void ControllerWidget::setAnyDlgVisible(bool enable)
{
	if (enable) {
		//ui.videoManager->show();
		ui.miniwidget->show();
		ui.toolbarWidget->SetAnyButtonVisible(true);
	}
	else {
		//ui.videoManager->hide();
		ui.miniwidget->hide();
		ui.toolbarWidget->SetAnyButtonVisible(false);
	}
}