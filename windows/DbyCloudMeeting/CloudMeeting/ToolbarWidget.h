#pragma once
#include <QWidget>
#include "ui_ToolbarWidget.h"
#include <QMenu>

class MyAction : public QAction
{
	Q_OBJECT
public:
	MyAction() {}
	~MyAction() {}

	void SetParam(int index, QString name) { m_index = index; m_name = name; }
	int GetIndex() { return m_index; }
	QString GetName() { return m_name; }
private:
	int m_index;
	QString m_name;
};

class ToolbarWidget : public QWidget
{
	Q_OBJECT

public:
	ToolbarWidget(QWidget *parent = Q_NULLPTR);
	~ToolbarWidget();

	void Init();
	void SetAnyButtonVisible(bool enable); //Ö±²¥
signals:
	void SigVideoListCtrl();
signals:
	void SigChatCtrl();
signals:
	void SigUserListCtrl();
signals:
	void SigLeaveChannel();

public slots:
	void OnSelectCameraCtrl();
	void OnMicrophoneCtrl();
	void OnQuitChannelCtrl();
	void OnSetVideoDevice();
	void OnEnableLocalVideo();
	void OnSetAudioDevice();
	void OnEnableLocalAudio();

private:
	void initDlg();
	void initVideoDevice();
	void initAudioDevice();

protected:
	//void showEvent(QShowEvent *event);
private:
	Ui::ToolbarWidget ui;
	bool m_vState = false;
	bool m_aState = false;

	QAction *m_pAudioAct = NULL;
	QAction *m_pVideoAct = NULL;
};
