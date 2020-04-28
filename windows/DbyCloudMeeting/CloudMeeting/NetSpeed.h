#pragma once
#include <QThread>
#include <Windows.h>
#include <winsock2.h> 
#include <iphlpapi.h>
#include <TlHelp32.h>

#pragma comment(lib, "Iphlpapi.lib")
class NetSpeed : public QThread
{
	Q_OBJECT

public:
	NetSpeed();
	~NetSpeed();


	void run() override;
	void Stop();
signals:
	void SigNetSpeed(QString);

private:
	void getNetSpeed();
private:
	
	volatile bool m_isRunning = false;
};
