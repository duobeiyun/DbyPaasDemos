#include "UserStateWidget.h"
#include <QDateTime>
#include <QDebug>
#include "ObjectManager.h"
#include "QEventDef.h"

UserStateWidget::UserStateWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	/*connect(ui.pushButton, &QPushButton::clicked,
		this, &UserStateWidget::OnUpdateUserState);*/


	this->setStyleSheet("background-color: rgba(0, 0, 0, 0.0)");

	ui.listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.listWidget->setFrameShape(QListWidget::NoFrame);

	ObjectManager *pObjMngr = ObjectManager::GetObjectManager();
	connect(pObjMngr, SIGNAL(SigUserOnlineState(QString, QString)), this, SLOT(OnUpdateUserState(QString, QString)));
	connect(pObjMngr, SIGNAL(SigUserOfflineState(QString, QString)), this, SLOT(OnUpdateUserState(QString, QString)));
	connect(pObjMngr, SIGNAL(SigUpdateStatusInformation(int, QString)), this, SLOT(OnUpdateStatusInformation(int, QString)));

}

UserStateWidget::~UserStateWidget()
{
}

//void UserStateWidget::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QWidget::showEvent(event);
//}

void UserStateWidget::Clear()
{
	while (ui.listWidget->count() > 0)
	{
		QListWidgetItem *item = ui.listWidget->takeItem(0);
		delete item;
	}

	ui.listWidget->clear();
}

void UserStateWidget::OnUpdateUserState(QString uid, QString msgInfo)
{
	QString time = QString::number(QDateTime::currentDateTime().toTime_t());

	QNChatMessage* messageW = new QNChatMessage(ui.listWidget->parentWidget());
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	dealMessage(messageW, item, msgInfo, time, QNChatMessage::User_Me);
	messageW->SetTextSuccess();
	messageW->SetUid(uid);
	ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
}

void UserStateWidget::OnUpdateStatusInformation(int event, QString info)
{
	QString time = QString::number(QDateTime::currentDateTime().toTime_t());
	QNChatMessage* messageW = new QNChatMessage(ui.listWidget->parentWidget());
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	dealMessage(messageW, item, info, time, QNChatMessage::User_Me);
	messageW->SetTextSuccess();
	messageW->SetUid(QString::fromLocal8Bit("系统"));
	ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
}

void UserStateWidget::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type)
{
	messageW->setFixedWidth(this->width());
	QSize size = messageW->FontRect(text);
	item->setSizeHint(size);
	messageW->SetText(text, time, size, type);
	ui.listWidget->setItemWidget(item, messageW);
}

void UserStateWidget::dealMessageTime(QString curMsgTime)
{
	bool isShowTime = false;
	if (ui.listWidget->count() > 0) {
		QListWidgetItem* lastItem = ui.listWidget->item(ui.listWidget->count() - 1);
		QNChatMessage* messageW = (QNChatMessage*)ui.listWidget->itemWidget(lastItem);
		int lastTime = messageW->time().toInt();
		int curTime = curMsgTime.toInt();
		qDebug() << "curTime lastTime:" << curTime - lastTime;
		isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
	}
	else {
		isShowTime = true;
	}
	if (isShowTime) {
		QNChatMessage* messageTime = new QNChatMessage(ui.listWidget->parentWidget());
		QListWidgetItem* itemTime = new QListWidgetItem(ui.listWidget);

		QSize size = QSize(this->width(), 40);
		messageTime->resize(size);
		itemTime->setSizeHint(size);
		messageTime->SetText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
		ui.listWidget->setItemWidget(itemTime, messageTime);
	}
}