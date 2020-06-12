#include "UserListWidget.h"
#include <QPushButton>
#include "NoFocusDelegate.h"
#include "ObjectManager.h"
#include "DbyObject.h"

UserListWidget::UserListWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initDialog();
}

UserListWidget::~UserListWidget()
{
}

//void UserListWidget::showEvent(QShowEvent *event)
//{
//	this->setAttribute(Qt::WA_Mapped);
//	QWidget::showEvent(event);
//}

void UserListWidget::AddLocalUid(QString uid)
{
	//local 先不加入列表
	//addWidget(uid);
}

void UserListWidget::Clear()
{
	ui.tableWidget->setRowCount(0);
}

void UserListWidget::OnUpdateUserList(QString uid, bool isAdd)
{
	QString strUid = uid;

	isAdd ? addWidget(strUid) : delWidget(strUid);
}

void UserListWidget::OnVideoButtonClicked()
{
	QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
	if (senderObj == nullptr)
		return;

	static bool isMute = false;
	QModelIndex idx = ui.tableWidget->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
	QTableWidgetItem *pItem = ui.tableWidget->item(idx.row(), 2);
	std::string uid = pItem->text().toStdString();
	int ret = DbyObject::GetDbyObject()->GetDbyRtcEngine()->muteRemoteVideoStream(uid.c_str(),"", isMute);

	setVideoBtStyle(senderObj, isMute);

	QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigVideoStreamMuteState", Q_ARG(QString, pItem->text()), Q_ARG(bool, isMute));

	isMute = !isMute;
}

void UserListWidget::OnAudioButtonClicked()
{
	QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
	if (senderObj == nullptr)
		return;

	static bool isMute = false;
	QModelIndex idx = ui.tableWidget->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
	QTableWidgetItem *pItem = ui.tableWidget->item(idx.row(), 2);
	std::string uid = pItem->text().toStdString();
	DbyObject::GetDbyObject()->GetDbyRtcEngine()->muteRemoteAudioStream(uid.c_str(), "", isMute);

	setAudioBtStyle(senderObj, isMute);

	//QMetaObject::invokeMethod(ObjectManager::GetObjectManager(), "SigAudioStreamMuteState", Q_ARG(QString, pItem->text()), Q_ARG(bool, isMute));

	isMute = !isMute;
}

void UserListWidget::initDialog()
{
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0.7)");

	ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.tableWidget->setFrameShape(QTableWidget::NoFrame);

	ui.tableWidget->verticalHeader()->setHidden(true);
	ui.tableWidget->horizontalHeader()->setHidden(true);
	ui.tableWidget->setColumnCount(3);
	//ui.tableWidget->setColumnWidth(0, 77);
	ui.tableWidget->setItemDelegate(new NoFocusDelegate());
	ui.tableWidget->setStyleSheet("selection-background-color:gray;");
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(24);
	ui.tableWidget->setColumnWidth(0, 40);
	ui.tableWidget->setColumnWidth(1, 40);
	ui.tableWidget->setColumnWidth(2, 200);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ObjectManager* pObjMngr = ObjectManager::GetObjectManager();
	ui.tableWidget->resizeColumnsToContents();

	
	connect(pObjMngr, SIGNAL(SigUpdateUserList(QString, bool)), this, SLOT(OnUpdateUserList(QString, bool)));
}

void UserListWidget::addWidget(QString uid)
{
	QPushButton *btV = new QPushButton;
	QPushButton *btA = new QPushButton;
	
	setVideoBtStyle(btV);
	setAudioBtStyle(btA);

	connect(btV, SIGNAL(clicked()), this, SLOT(OnVideoButtonClicked()));
	connect(btA, SIGNAL(clicked()), this, SLOT(OnAudioButtonClicked()));

	int rowCount = ui.tableWidget->rowCount();

	ui.tableWidget->insertRow(rowCount);
	ui.tableWidget->setCellWidget(rowCount, 0, btV);
	ui.tableWidget->setCellWidget(rowCount, 1, btA);

	QTableWidgetItem *item = new QTableWidgetItem(QString("%1").arg(uid));
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item->setFlags(Qt::ItemIsEditable);
	item->setForeground(QColor(255, 255, 255));
	ui.tableWidget->setItem(rowCount, 2, item);
}

void UserListWidget::delWidget(QString uid)
{
	int row = findRow(uid);
	if (row < 0)
		return;

	QTableWidgetItem *item = ui.tableWidget->item(row, 2);
	if (item->text() == uid)
		ui.tableWidget->removeRow(row);
}

int UserListWidget::findRow(QString uid)
{
	int row = -1;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		QTableWidgetItem *item = ui.tableWidget->item(i, 2);
		if (item->text() == uid)
		{
			row = i;
			break;
		}
	}

	return row;
}

void UserListWidget::setVideoBtStyle(QPushButton* pb, bool isopen)
{
	pb->setIconSize(QSize(24, 24));
	pb->setIcon(QIcon(isopen ? "./Resources/icon-camera-open-normal-72-72.b50a8f7d.png": "./Resources/icon-camera-closed-normal-72-72.f65f0cc1.png"));	
}

void UserListWidget::setAudioBtStyle(QPushButton* pb, bool isopen)
{
	pb->setIconSize(QSize(24, 24));
	pb->setIcon(QIcon(isopen ? "./Resources/icon-mic-open-normal-72-72.32f13f3d.png" : "./Resources/icon-mic-closed-normal-72-72.b7fe5142.png"));
}