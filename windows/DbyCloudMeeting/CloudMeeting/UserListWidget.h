#pragma once

#include <QWidget>
#include <QPushButton>
#include "ui_UserListWidget.h"

class UserListWidget : public QWidget
{
	Q_OBJECT

public:
	UserListWidget(QWidget *parent = Q_NULLPTR);
	~UserListWidget();

	void AddLocalUid(QString uid);
	void Clear();
public slots:
	void OnUpdateUserList(QString uid, bool isAdd);
	void OnVideoButtonClicked();
	void OnAudioButtonClicked();
protected:
	//void showEvent(QShowEvent *event);

private:
	void initDialog();
	void addWidget(QString uid);
	void delWidget(QString uid);
	int findRow(QString);

	void setVideoBtStyle(QPushButton* pb, bool isopen = true);
	void setAudioBtStyle(QPushButton* pb, bool isopen = true);

private:
	Ui::UserListWidget ui;
};
