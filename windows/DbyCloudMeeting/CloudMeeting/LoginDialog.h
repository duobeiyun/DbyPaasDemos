#pragma once

#include <QDialog>
#include <QMouseEvent>
#include "ui_LoginDialog.h"

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = Q_NULLPTR);
	~LoginDialog();

signals:
	void SigJoinChannel(QString channelId, QString userId, QString nickname);

public slots:
	void OnDlgMin();
	void OnDlgClose();
	void OnJoinChannel();
	void OnLeaveChannl();
	void OnSelRoute(bool isChecked);
	void OnJoinPlayBack();
	void OnCheckBoxStateChanged(int);
protected:
	void keyPressEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	
	//void showEvent(QShowEvent *event);
private:
	void initDialog();

private:
	Ui::LoginDialog ui;
	QPoint m_start;
	QPoint m_clickPos;
	bool m_isAgora = false;
	bool m_leftButtonPressed = false;
};
