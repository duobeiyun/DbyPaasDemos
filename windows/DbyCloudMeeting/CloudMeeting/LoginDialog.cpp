#include "LoginDialog.h"
#include <QDesktopWidget>
#include <QRegExp>
#include <QValidator>
#include "DbMainWindow.h"
#include "DbSettings.h"
#include "QEventDef.h"
#include <QProcess>

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initDialog();
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::OnDlgMin()
{
	this->showMinimized();
}

void LoginDialog::OnDlgClose()
{
	this->close();
}

void LoginDialog::OnJoinChannel()
{
	if (ui.channelIdLEdit->text().trimmed().isEmpty())
	{
		ui.inputErrorTipLabel->setText(QString::fromLocal8Bit("输入不能为空"));
		return;
	}

	ui.inputErrorTipLabel->setText(QString(""));

	this->hide();

	QString strChannelId = ui.channelIdLEdit->text();
	QString strUserId = ui.userIdLEdit->text();

	DbSettings::Write(CID, strChannelId, LOGIN);
	DbSettings::Write(UID, strUserId, LOGIN);

	/*CloudMeeting *w = new CloudMeeting;
	connect(w, SIGNAL(SigLeaveChannel()), this, SLOT(OnLeaveChannl()), Qt::DirectConnection);
	bool isDualStream = ui.checkBox->isChecked();

	w->OnJoinChannel(strChannelId, strUserId, strUserId, isDualStream);*/
	bool isDualStream = ui.checkBox->isChecked();
	if (m_isAgora)
	{
		QProcess myProcess;
		QString qexeFullPath = QCoreApplication::applicationDirPath();
		qexeFullPath += "/AgoraCloudMeeting.exe";
		qexeFullPath = "\"" + qexeFullPath + "\"";
		QStringList arguments;
		arguments << strUserId << strChannelId << (isDualStream ? QString("true") : QString("false"));
		myProcess.start(qexeFullPath, arguments);
		while (myProcess.waitForFinished(100) == false) {
		}
		this->show();
	}
	else {
		DbMainWindow *w = new DbMainWindow;
		connect(w, SIGNAL(SigLeaveChannel()), this, SLOT(OnLeaveChannl()), Qt::DirectConnection);
		bool profile = ui.checkBox_profile->isChecked();
		bool role = ui.checkBox_role->isChecked();
		w->JoinChannel(strChannelId, strUserId, strUserId, isDualStream, profile, role);
	}
}

void LoginDialog::OnLeaveChannl()
{
	this->show();
}

void LoginDialog::OnSelRoute(bool isCheck)
{
	m_isAgora = ui.AgoraRdb->isChecked();
}

void LoginDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier) && event->key() == Qt::Key_N)
	{
		ui.AgoraRdb->show();
		ui.duobeiRdb->show();
		ui.joinPlaybackBt->show();
	}
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;
		m_start = event->globalPos();
		m_clickPos = event->pos();
	}
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (m_leftButtonPressed)
	{
		move(event->globalPos() - m_clickPos /*- QPoint(0, 0)*/);	//TODO
		m_start = event->globalPos();
	}
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
	}
}

void LoginDialog::initDialog()
{
	int iWidth = this->width();
	int iHeight = this->height();
	this->setWindowFlags(Qt::FramelessWindowHint);
	ui.maxBt->setEnabled(false);

	QString strchlid, struid;
	DbSettings::Read(CID, strchlid, LOGIN);
	DbSettings::Read(UID, struid, LOGIN);

	ui.channelIdLEdit->setText(strchlid);
	ui.userIdLEdit->setText(struid);

	QRegExp regx("[a-zA-Z0-9]+$");
	QValidator* validator = new QRegExpValidator(regx, NULL);
	ui.channelIdLEdit->setValidator(validator);
	ui.userIdLEdit->setValidator(validator);
	//ui.userIdLEdit->setMaxLength(9);

	ui.versionLabel->setText(VERSION);

	qDebug() << "version = " << VERSION;

	connect(ui.joinChannelBt, SIGNAL(clicked()), this, SLOT(OnJoinChannel()));
	connect(ui.miniBt, SIGNAL(clicked()), this, SLOT(OnDlgMin()));
	connect(ui.closeBt, SIGNAL(clicked()), this, SLOT(OnDlgClose()));
	connect(ui.AgoraRdb, SIGNAL(toggled(bool)), this, SLOT(OnSelRoute(bool)));
	connect(ui.joinPlaybackBt, SIGNAL(clicked()), this, SLOT(OnJoinPlayBack()));
	connect(ui.checkBox_profile, SIGNAL(stateChanged(int)), this, SLOT(OnCheckBoxStateChanged(int)));
	

	ui.duobeiRdb->setChecked(true);
	ui.AgoraRdb->hide();
	ui.duobeiRdb->hide();
	ui.joinPlaybackBt->hide();
	ui.checkBox_profile->setChecked(false);
	ui.checkBox_role->setChecked(false);
	ui.checkBox->setChecked(false);

	ui.checkBox_role->hide();
}

//void LoginDialog::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QDialog::showEvent(event);
//}

void LoginDialog::OnJoinPlayBack()
{
	this->hide();
	QProcess myProcess;
	QString qexeFullPath = QCoreApplication::applicationDirPath();
	qexeFullPath += "/DbyPlaybackClient.exe";
	/*QString qexeFullPath = QString::fromLocal8Bit("D:/Program Files (x86)/多贝云会议（PC）/DbyPlaybackClient.exe");*/
	qexeFullPath = "\"" + qexeFullPath + "\"";
	myProcess.start(qexeFullPath);
	while (myProcess.waitForFinished(100) == false) {
		qDebug() << "000000000000000";
	}
	this->show();
}

void LoginDialog::OnCheckBoxStateChanged(int)
{
	if (ui.checkBox_profile->isChecked()) {
		ui.checkBox->setChecked(false);
		ui.checkBox->hide();
		ui.checkBox_role->show();
	}
	else {
		ui.checkBox_role->hide();
		ui.checkBox->show();
	}
}