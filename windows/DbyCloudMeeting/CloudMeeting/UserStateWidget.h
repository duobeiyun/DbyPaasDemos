#pragma once

#include <QWidget>
#include "ui_UserStateWidget.h"
#include "QNChatMessage/QNChatMessage.h"
class UserStateWidget : public QWidget
{
	Q_OBJECT

public:
	UserStateWidget(QWidget *parent = Q_NULLPTR);
	~UserStateWidget();

	void Clear();
public slots:
	void OnUpdateUserState(QString uid, QString msg);
	void OnUpdateStatusInformation(int event, QString info);
protected:
	//void showEvent(QShowEvent *event);
private:
	void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
	void dealMessageTime(QString curMsgTime);
	
private:
	Ui::UserStateWidget ui;
};
