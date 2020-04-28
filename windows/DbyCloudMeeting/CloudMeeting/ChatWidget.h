#pragma once

#include <QWidget>
#include "ui_ChatWidget.h"
#include "QNChatMessage/QNChatMessage.h"
class ChatWidget : public QWidget
{
	Q_OBJECT

public:
	ChatWidget(QWidget *parent = Q_NULLPTR);
	~ChatWidget();

public slots:
	void OnSendMsgClicked();
	void OnRemoteUserChatMsg(QString uid, QString msg);

protected:
	void resizeEvent(QResizeEvent *event);
private:
	void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
	void dealMessageTime(QString curMsgTime);

private:
	Ui::ChatWidget ui;
};
