#include "DbMainWindow.h"
#include <QByteArray>
#include "DbyObject.h"
#include "ObjectManager.h"

DbMainWindow::DbMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initDlg();
}

DbMainWindow::~DbMainWindow()
{
	qDebug() << "DbMainWindow::~DbMainWindow()";
	m_Controller->deleteLater();
	m_Controller = nullptr;
}

void DbMainWindow::JoinChannel(QString channelId, QString userId, QString nickname, bool isDualStream, bool profile, bool role)
{
	this->showMaximized();
	ui.openGLWidget->SetUid(userId);
	m_Controller->OnJoinChannel(channelId, userId, nickname, isDualStream, profile, role);
}

void DbMainWindow::OnProcessShowMain(const QByteArray& ba, uint w, uint h)
{
	int res = QMetaObject::invokeMethod(ui.openGLWidget, "SlotShowYuv", Q_ARG(const QByteArray&, ba), Q_ARG(uint, w), Q_ARG(uint, h));
}

void DbMainWindow::OnSetMainUid(QString uid)
{
	ui.openGLWidget->SetUid(uid);
}

void DbMainWindow::OnLeaveChannel()
{
	m_nQuitState = CLOSE_EVENT_STATE_CLOSING;
	emit SigLeaveChannel();
	this->close();
}

void DbMainWindow::initDlg()
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowIcon(QIcon("./Resources/duobei.ico"));
	this->setWindowTitle(QString::fromLocal8Bit("多贝云会议"));

	ui.dbmainwidget->setStyleSheet("background:rgb(28, 28, 28)");

	m_pBgWgt = new CameraImageWidget(ui.dbmainwidget);
	m_pBgWgt->setObjectName("mainShowCameraPic");
	ui.gridLayout->addWidget(m_pBgWgt, 0, 0, 1, 1);
	ui.openGLWidget->hide();

	m_Controller = new ControllerWidget(this);
	ui.gridLayout_2->addWidget(m_Controller, 0, 0, 0, 0);

	connect(m_Controller, SIGNAL(SigShowMain(const QByteArray&, uint, uint)), this, SLOT(OnProcessShowMain(const QByteArray&, uint, uint)));
	connect(m_Controller, SIGNAL(SigSetMainUid(QString)), this, SLOT(OnSetMainUid(QString)));
	connect(m_Controller, SIGNAL(SigLeaveChannel()), this, SLOT(OnLeaveChannel()));
	connect(m_Controller, SIGNAL(SigQuitLoading()), this, SLOT(OnQuitLoading()));
	
	connect(ObjectManager::GetObjectManager(), SIGNAL(SigChangeLocalStreamState()), this, SLOT(OnChangeLocalStreamState()));
	connect(ObjectManager::GetObjectManager(), SIGNAL(SigChangeRemoteStreamState(QString)), this, SLOT(OnChangeRemoteStreamState(QString)));
}

void DbMainWindow::showEvent(QShowEvent *event)
{
	this->setAttribute(Qt::WA_Mapped);
	QMainWindow::showEvent(event);
}

void DbMainWindow::closeEvent(QCloseEvent *event)
{
	//m_Controller->OnLeaveChannel();
	//OnLeaveChannel();
	if (m_nQuitState == CLOSE_EVENT_STATE_INIT)
	{
		OnQuitLoading();
		int res = QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigLeaveChannel", Qt::DirectConnection);
		event->ignore();
		m_nQuitState = CLOSE_EVENT_STATE_OTHERS;
	}
	else if(m_nQuitState == CLOSE_EVENT_STATE_CLOSING){
		event->accept();
		m_nQuitState = CLOSE_EVENT_STATE_INIT;
	}
	else {
		event->ignore();
	}
}

void DbMainWindow::OnChangeLocalStreamState()
{
	if (m_isQuitLoading)
		return;

	ObjectManager* obj = ObjectManager::GetObjectManager();
	int place = obj->GetLocalShowPlace();
	bool bvideo = obj->GetLocalVideoStreamState();
	bool baudio = obj->GetLocalAudioStreamState();
	if (place == 0) {
		if (bvideo) {
			ui.openGLWidget->show();
			m_pBgWgt->hide();
		}
		else {
			ui.openGLWidget->hide();
			m_pBgWgt->show();
			m_pBgWgt->Show(false);
		}

		m_Controller->SetLocalMainAudioState(baudio);	
	}
	else {
		m_Controller->SetMinLocalShowState(bvideo);
	}
}

void DbMainWindow::OnChangeRemoteStreamState(QString uid)
{
	if (m_isQuitLoading)
		return;
	//获取
	if (!m_Controller)
		return;

	STRU_REMOTE_USER info;
	m_Controller->GetRemoteStreamState(uid, info);

	//设置大流有效
	if (info.isvideoplayeropen && info.state == USER_JOIN_MAIN_STATE) {
		DbyObject::GetDbyObject()->SetDualStream(uid.toStdString().c_str(), 1);
	}

	if (info.isvideomuteopen && info.isvideoplayeropen) {
		if (info.isFirstFrame) {
			ui.openGLWidget->show();
			m_pBgWgt->hide();
		}
		else {
			ui.openGLWidget->hide();
			m_pBgWgt->show();
			m_pBgWgt->Show(true);
		}
		
	}
	else {
		ui.openGLWidget->hide();
		m_pBgWgt->show();
		m_pBgWgt->Show(false);
	}
}

void DbMainWindow::OnQuitLoading()
{
	m_isQuitLoading = true;
	ui.openGLWidget->hide();
	m_pBgWgt->show();
	m_pBgWgt->Show(true);
}