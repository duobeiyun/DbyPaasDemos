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

	DWORD   dwLastIn = 0;           //��һ���ӵĽ����ֽ���
	DWORD   dwLastOut = 0;          //��һ���ӵķ����ֽ���
	DWORD   dwBandIn = 0;           //�����ٶ�
	DWORD   dwBandOut = 0;          //�ϴ��ٶ�

	QString strNetSpeed;

	while (m_isRunning) {
		strNetSpeed.clear();
		GetIfTable(pTable, &m_dwAdapters, TRUE);
		DWORD   dwInOctets = 0;
		DWORD   dwOutOctets = 0;

		//�����ж˿ڵ���������ͳ��
		for (UINT i = 0; i < pTable->dwNumEntries; i++)
		{
			MIB_IFROW   Row = pTable->table[i];
			dwInOctets += Row.dwInOctets;
			dwOutOctets += Row.dwOutOctets;
		}

		dwBandIn = dwInOctets - dwLastIn;       //�����ٶ�
		dwBandOut = dwOutOctets - dwLastOut;    //�ϴ�����
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

		strNetSpeed = QString::fromLocal8Bit("����: %1 Kb/s ����: %2 kb/s").arg(dwBandIn).arg(dwBandOut);
		emit SigNetSpeed(strNetSpeed);
		Sleep(1000);
	}

	delete[] pTable;
}