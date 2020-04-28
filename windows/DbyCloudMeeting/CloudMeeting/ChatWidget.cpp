#include "ChatWidget.h"
#include <QDateTime>
#include <QDebug>
#include "ObjectManager.h"
#include "DbyObject.h"
ChatWidget::ChatWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->setStyleSheet("background-color: rgba(0, 0, 0, 0.7)");

	ui.listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.listWidget->setFrameShape(QListWidget::NoFrame);

	ui.chatmsglineEdit->setStyleSheet("background-color:rgb(255,255,255)");
	ui.sendButton->setStyleSheet("background-color:rgb(57, 119, 246)");

	connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(OnSendMsgClicked()));
	ObjectManager *pObj = ObjectManager::GetObjectManager();
	connect(pObj, SIGNAL(SigRemoteUserChatMsg(QString, QString)), this, SLOT(OnRemoteUserChatMsg(QString, QString)));
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::OnSendMsgClicked()
{
	QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
	QString msg = ui.chatmsglineEdit->text();

	if (msg.isEmpty())
		return;

	QNChatMessage* messageW = new QNChatMessage(ui.listWidget->parentWidget());

	QString strUid = ObjectManager::GetObjectManager()->GetLocalUid();
	messageW->SetUid(strUid);
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
	messageW->SetTextSuccess();

	ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);

	DbyObject::GetDbyObject()->SendChatMsg(msg.toStdString().c_str());
	ui.chatmsglineEdit->clear();
}

void ChatWidget::OnRemoteUserChatMsg(QString uid, QString msg)
{
	//解析msg
	QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
	ui.chatmsglineEdit->setText("");
	if (msg != "") {
		QNChatMessage* messageW = new QNChatMessage(ui.listWidget->parentWidget());
		QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
		messageW->SetUid(uid);
		dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
	}

	ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
}

void ChatWidget::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);


	/*ui.lineEdit->resize(this->width() - 20, ui.widget->height() - 20);
	ui.lineEdit->move(10, 10);

	ui.pushButton->move(ui.lineEdit->width() + ui.lineEdit->x() - ui.pushButton->width() - 10,
		ui.lineEdit->height() + ui.lineEdit->y() - ui.pushButton->height() - 10);


	for (int i = 0; i < ui.listWidget->count(); i++) {
		QNChatMessage* messageW = (QNChatMessage*)ui.listWidget->itemWidget(ui.listWidget->item(i));
		QListWidgetItem* item = ui.listWidget->item(i);

		dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
	}*/
}

void ChatWidget::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type)
{
	messageW->setFixedWidth(this->width());
	QSize size = messageW->FontRect(text);
	item->setSizeHint(size);
	messageW->SetText(text, time, size, type);
	ui.listWidget->setItemWidget(item, messageW);
}

void ChatWidget::dealMessageTime(QString curMsgTime)
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