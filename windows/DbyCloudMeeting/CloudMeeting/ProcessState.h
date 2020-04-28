#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <direct.h>
#include <winternl.h>
#include <Psapi.h>
#include <assert.h>  
#include <QMap>
#include <QThread>
#include <QTime>
#include <QTimer>

#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"Psapi.lib")

typedef long long           int64_t;
typedef unsigned long long  uint64_t;

class ProcessState : public QThread
{
	Q_OBJECT

public:
	ProcessState();
	~ProcessState();
	void Stop();
	void run()override;
signals:
	void SigCpuMemUsageRate(int cpu, int mem, QString tm);
public slots:
	void OnTimeOut();

private:
	inline char* unicodeToAnsi(const wchar_t* szStr)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
		if (nLen == 0)
		{
			return NULL;
		}
		char* pResult = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
		return pResult;
	}

	__int64 compareFileTime(FILETIME preTime, FILETIME nowTime);
	__int64 filetime2Int64(const FILETIME* ftime);
	
	QMap<int, QString> getAllProcess();
	bool getProcessMemory(int nPid, int& nProcessMemRate);
	bool getSysCpu(int& nCpuRate);
	bool getSysDisk(int& nDiskTotal, int& nDiskUsed);
	bool getSysMemory(int& nMemTotal, int& nMemUsed);

	int getCpuUsage();
	int getIoBytes(uint64_t* read_bytes, uint64_t* write_bytes);
	int getMemoryUsage(uint64_t* mem, uint64_t* vmem);
	QString formatTime(int ms);
private:
	QTimer *m_pTimer = NULL;
	QTime m_time;
	volatile bool m_isRunning = false;
};
