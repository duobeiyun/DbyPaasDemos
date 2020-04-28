#include "ToolbarWidget.h"
#include <Windows.h>
#include "DbyObject.h"
#include "ObjectManager.h"

std::string StringToUtf(std::string strValue)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//¼ÓÉÏÄ©Î²'\0'
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), strValue.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr = pBuf;
	delete[]pBuf;
	delete[]pwBuf;
	return retStr;
}

ToolbarWidget::ToolbarWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0.7)");
	initDlg();

	connect(ui.videoListCtrlBt, SIGNAL(clicked()), this, SIGNAL(SigVideoListCtrl()));
	connect(ui.selectCameraBt, SIGNAL(clicked()), this, SLOT(OnSelectCameraCtrl()));
	connect(ui.selectMicroBt, SIGNAL(clicked()), this, SLOT(OnMicrophoneCtrl()));
	connect(ui.chatCtrlBt, SIGNAL(clicked()), this, SIGNAL(SigChatCtrl()));
	connect(ui.userListCtrlBt, SIGNAL(clicked()), this, SIGNAL(SigUserListCtrl()));
	connect(ui.quitChannelCtrlBt, SIGNAL(clicked()), this, SLOT(OnQuitChannelCtrl()));
}

ToolbarWidget::~ToolbarWidget()
{
}

//void ToolbarWidget::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QWidget::showEvent(event);
//}

void ToolbarWidget::Init()
{
	initVideoDevice();
	initAudioDevice();
}

void ToolbarWidget::OnSelectCameraCtrl()
{

}

void ToolbarWidget::OnMicrophoneCtrl()
{

}

void ToolbarWidget::OnQuitChannelCtrl()
{
	emit SigLeaveChannel();
}

void ToolbarWidget::OnSetVideoDevice()
{
	MyAction *senderObj = qobject_cast<MyAction*>(sender());
	if (senderObj == nullptr)
		return;

	DbyObject::GetDbyObject()->SetCamera(senderObj->GetName().toStdString().c_str());
	ObjectManager::GetObjectManager()->SetLocalVideoStreamState(true);
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");
	m_vState = false;
	if(m_pVideoAct)
		m_pVideoAct->setText(m_vState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));

}

void ToolbarWidget::OnEnableLocalVideo()
{
	DbyObject::GetDbyObject()->EnableLocalVideo(m_vState);
	ObjectManager::GetObjectManager()->SetLocalVideoStreamState(m_vState);
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");
	m_vState = !m_vState;
	if (m_pVideoAct)
		m_pVideoAct->setText(m_vState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
}

void ToolbarWidget::OnSetAudioDevice()
{
	MyAction *senderObj = qobject_cast<MyAction*>(sender());
	if (senderObj == nullptr)
		return;

	QString strname = senderObj->GetName();
	std::string devname = strname.toLocal8Bit();
	DbyObject::GetDbyObject()->SetMicrophone(devname.c_str());
	ObjectManager::GetObjectManager()->SetLocalAudioStreamState(true);
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");
	m_aState = false;
	if(m_pAudioAct)
		m_pAudioAct->setText(m_aState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
}

void ToolbarWidget::OnEnableLocalAudio()
{
	DbyObject::GetDbyObject()->EnableLocalAudio(m_aState);
	ObjectManager::GetObjectManager()->SetLocalAudioStreamState(m_aState);
	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigChangeLocalStreamState");
	m_aState = !m_aState;
	if (m_pAudioAct)
		m_pAudioAct->setText(m_aState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
}

void ToolbarWidget::initVideoDevice()
{
	QMenu *pMenu = new QMenu(ui.videoListCtrlBt);

	char dnTemp[MAXSIZE];
	//pMenu->setStyleSheet();
	for (int i = 0; i < DbyObject::GetDbyObject()->GetCameraCount(); i++)
	{
		QString deviceName;


		memset(dnTemp, '/0', sizeof(dnTemp));
		DbyObject::GetDbyObject()->GetCamera(i, dnTemp, MAXSIZE);

		deviceName = QString::fromUtf8(dnTemp);

		MyAction *pAction = new MyAction;
		pAction->setText(deviceName);
		pAction->SetParam(i, deviceName);
		connect(pAction, SIGNAL(triggered()), this, SLOT(OnSetVideoDevice()));
		pMenu->addAction(pAction);
	}

	m_pVideoAct = new QAction;
	m_pVideoAct->setText(m_vState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
	connect(m_pVideoAct, SIGNAL(triggered()), this, SLOT(OnEnableLocalVideo()));

	pMenu->addAction(m_pVideoAct);

	ui.selectCameraBt->setMenu(pMenu);
}



void ToolbarWidget::initAudioDevice()
{
	QMenu *pMenu = new QMenu(ui.videoListCtrlBt);

	//pMenu->setStyleSheet();
	for (int i = 0; i < DbyObject::GetDbyObject()->GetMicrophoneCount(); i++)
	{
		QString deviceName;

		char temp[MAXSIZE];
		memset(temp, '/0', sizeof(temp));
		DbyObject::GetDbyObject()->GetMicrophone(i, temp, MAXSIZE);
		//deviceName = QString::fromUtf8(temp);
		deviceName = QString::fromLocal8Bit(temp);

		MyAction *pAction = new MyAction;
		pAction->setText(deviceName);
		pAction->SetParam(i, deviceName);
		connect(pAction, SIGNAL(triggered()), this, SLOT(OnSetAudioDevice()));
		pMenu->addAction(pAction);
	}

	m_pAudioAct = new QAction;
	m_pAudioAct->setText(m_aState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
	connect(m_pAudioAct, SIGNAL(triggered()), this, SLOT(OnEnableLocalAudio()));

	pMenu->addAction(m_pAudioAct);

	ui.selectMicroBt->setMenu(pMenu);
}

void ToolbarWidget::initDlg()
{
	ui.flushBt->hide();
	ui.videoListCtrlBt->setIconSize(QSize(24, 24));
	ui.videoListCtrlBt->setIcon(QIcon("./Resources/icon-side-bar-active-72-72.ca9c69a2.png"));

	ui.selectCameraBt->setIconSize(QSize(24, 24));
	ui.selectCameraBt->setIcon(QIcon("./Resources/icon-camera-open-normal-72-72.b50a8f7d.png"));

	ui.selectMicroBt->setIconSize(QSize(24, 24));
	ui.selectMicroBt->setIcon(QIcon("./Resources/icon-mic-open-normal-72-72.32f13f3d.png"));

	ui.chatCtrlBt->setIconSize(QSize(24, 24));
	ui.chatCtrlBt->setIcon(QIcon("./Resources/icon-message-active-72-72.99f01a90.png"));

	ui.userListCtrlBt->setIconSize(QSize(24, 24));
	ui.userListCtrlBt->setIcon(QIcon("./Resources/icon-users-active-72-72.215b0024.png"));

	ui.quitChannelCtrlBt->setIconSize(QSize(24, 24));
	ui.quitChannelCtrlBt->setIcon(QIcon("./Resources/icon-hang-up-72-72.7c53bc2c.png"));
}

void ToolbarWidget::SetAnyButtonVisible(bool enable)
{
	if (enable) {
		ui.videoListCtrlBt->show();
		ui.selectCameraBt->show();
		ui.selectMicroBt->show();

		m_aState = !enable;
		m_vState = !enable;
		m_pAudioAct->setText(m_aState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
		m_pVideoAct->setText(m_vState ? QString::fromLocal8Bit("¿ªÆô") : QString::fromLocal8Bit("½ûÖ¹"));
	}
	else {
		ui.videoListCtrlBt->hide();
		ui.selectCameraBt->hide();
		ui.selectMicroBt->hide();
	}
}