#include "NetSpeed.h"
#include <QDebug>
NetSpeed::NetSpeed()
{
}

NetSpeed::~NetSpeed()
{
	qDebug() << "~NetSpeed()";
}

void NetSpeed::run()
{
	m_isRunning = true;
	getNetSpeed();
}

void NetSpeed::Stop()
{
	m_isRunning = false;
	this->quit();
	while (this->isRunning()) {
		Sleep(5);
	}
}

void NetSpeed::getNetSpeed()
{

	PMIB_IFTABLE pTable = nullptr;
	DWORD    m_dwAdapters = 0;
	ULONG    uRetCode = GetIfTable(pTable, &m_dwAdapters, TRUE);
	if (uRetCode == ERROR_NOT_SUPPORTED)
	{
		return;
	}

	if (uRetCode == ERROR_INSUFFICIENT_BUFFER)
	{
		pTable = (PMIB_IFTABLE)new BYTE[65535];
	}

	DWORD   dwLastIn = 0;           //上一秒钟的接收字节数
	DWORD   dwLastOut = 0;          //上一秒钟的发送字节数
	DWORD   dwBandIn = 0;           //下载速度
	DWORD   dwBandOut = 0;          //上传速度

	QString strNetSpeed;

	while (m_isRunning) {
		strNetSpeed.clear();
		GetIfTable(pTable, &m_dwAdapters, TRUE);
		DWORD   dwInOctets = 0;
		DWORD   dwOutOctets = 0;

		//将所有端口的流量进行统计
		for (UINT i = 0; i < pTable->dwNumEntries; i++)
		{
			MIB_IFROW   Row = pTable->table[i];
			dwInOctets += Row.dwInOctets;
			dwOutOctets += Row.dwOutOctets;
		}

		dwBandIn = dwInOctets - dwLastIn;       //下载速度
		dwBandOut = dwOutOctets - dwLastOut;    //上传速速
		if (dwLastIn <= 0)
		{
			dwBandIn = 0;
		}
		else
		{
			dwBandIn = dwBandIn / 1024; //kb
		}

		if (dwLastOut <= 0)
		{
			dwBandOut = 0;
		}
		else
		{
			dwBandOut = dwBandOut / 1024;   //kb
		}

		dwLastIn = dwInOctets;
		dwLastOut = dwOutOctets;

		strNetSpeed = QString::fromLocal8Bit("下行: %1 Kb/s 上行: %2 kb/s").arg(dwBandIn).arg(dwBandOut);
		emit SigNetSpeed(strNetSpeed);
		Sleep(1000);
	}

	delete[] pTable;
}