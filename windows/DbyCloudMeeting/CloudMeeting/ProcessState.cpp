#include "ProcessState.h"
#include <QDebug>
#include <QEventLoop>
ProcessState::ProcessState()
{
}

ProcessState::~ProcessState()
{
	qDebug() << "~ProcessState()";
}

__int64 ProcessState::filetime2Int64(const FILETIME* ftime)
{
	LARGE_INTEGER li;
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

__int64 ProcessState::compareFileTime(FILETIME preTime, FILETIME nowTime)
{
	return this->filetime2Int64(&nowTime) - this->filetime2Int64(&preTime);
}

bool ProcessState::getSysCpu(int& nCpuRate)
{
	HANDLE hEvent;
	bool res;
	static FILETIME preIdleTime;
	static FILETIME preKernelTime;
	static FILETIME preUserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	preIdleTime = idleTime;
	preKernelTime = kernelTime;
	preUserTime = userTime;

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	WaitForSingleObject(hEvent, 500);

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	int idle = compareFileTime(preIdleTime, idleTime);
	int kernel = compareFileTime(preKernelTime, kernelTime);
	int user = compareFileTime(preUserTime, userTime);

	nCpuRate = (int)ceil(100.0*(kernel + user - idle) / (kernel + user));

	return res;
}

bool ProcessState::getSysMemory(int& nMemTotal, int& nMemUsed)
{
	MEMORYSTATUSEX memsStat;
	memsStat.dwLength = sizeof(memsStat);
	if (!GlobalMemoryStatusEx(&memsStat))
	{
		nMemTotal = -1;
		nMemUsed = -1;
		return false;
	}
	int nMemFree = memsStat.ullAvailPhys / (1024.0*1024.0);
	nMemTotal = memsStat.ullTotalPhys / (1024.0*1024.0);
	nMemUsed = nMemTotal - nMemFree;
	return true;
}

bool ProcessState::getSysDisk(int& nDiskTotal, int& nDiskUsed)
{
	static char path[_MAX_PATH];
	int curdrive = _getdrive();
	unsigned long lFreeAll = 0UL;
	unsigned long lTotalAll = 0UL;
	for (int drive = 1; drive <= 26; drive++)
	{
		if (!_chdrive(drive))
		{
			sprintf(path, "%c:\\", drive + 'A' - 1);
			ULARGE_INTEGER caller, total, free;
			WCHAR wszClassName[_MAX_PATH];
			memset(wszClassName, 0, sizeof(wszClassName));
			MultiByteToWideChar(CP_ACP, 0, path, strlen(path) + 1, wszClassName,
				sizeof(wszClassName) / sizeof(wszClassName[0]));
			if (GetDiskFreeSpaceEx(wszClassName, &caller, &total, &free) == 0)
			{
				qDebug() << "GetDiskFreeSpaceEx Filed!";
				return false;
			}

			double dTepFree = free.QuadPart / (1024.0*1024.0);
			double dTepTotal = total.QuadPart / (1024.0*1024.0);
			
			lFreeAll += (unsigned long)ceil(dTepFree);
			lTotalAll += (unsigned long)ceil(dTepTotal);
		}
	}
	nDiskUsed = lFreeAll;
	nDiskTotal = lTotalAll;
	return true;
}

QMap<int, QString> ProcessState::getAllProcess()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	QMap<int, QString> pidMap;
	
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		qDebug() << "CreateToolhelp32Snapshot Failed!";
		if (NULL != hProcessSnap)
		{
			CloseHandle(hProcessSnap);
			hProcessSnap = NULL;
		}
		return pidMap;
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		qDebug() << "Process32First Failed!";
		if (NULL != hProcessSnap)
		{
			CloseHandle(hProcessSnap);
			hProcessSnap = NULL;
		}
		return pidMap;
	}
	
	pidMap.clear();
	do
	{
		if (!pidMap.contains((int)pe32.th32ProcessID))
		{
			QString strProcessName = unicodeToAnsi(pe32.szExeFile);
			pidMap.insert((int)pe32.th32ProcessID, strProcessName);
		}

	
	} while (Process32Next(hProcessSnap, &pe32));
	if (NULL != hProcessSnap)
	{
		CloseHandle(hProcessSnap);
		hProcessSnap = NULL;
	}
	return pidMap;
}

bool ProcessState::getProcessMemory(int nPid, int& nProcessMemRate)
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	if (NULL == hProcess)
	{
		qDebug() << "OpenProcess Failed!";
		return false;
	}
	if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		qDebug() << "GetProcessMemoryInfo Failed!";
		return false;
	}

	int nMemTotal = 0;
	int nMemUsed = 0;
	this->getSysMemory(nMemTotal, nMemUsed);
	double tep = ((int)pmc.WorkingSetSize*1.0);
	nProcessMemRate = (int)ceil(100 * (tep / (1024.0*1024.0)) / nMemTotal);

	CloseHandle(hProcess);

	return true;
}
 
static uint64_t file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	assert(ftime);
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

static int get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

int ProcessState::getCpuUsage()
{ 
	static int processor_count_ = -1;
	static int64_t last_time_ = 0;
	static int64_t last_system_time_ = 0;


	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	int cpu = -1;


	if (processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
		&kernel_time, &user_time))
	{
			return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))

		/
		processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	assert(time_delta != 0);

	if (time_delta == 0)
		return -1;

	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}

int ProcessState::getMemoryUsage(uint64_t* mem, uint64_t* vmem)
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		if (mem)
			*mem = pmc.WorkingSetSize;
		if (vmem)
			*vmem = pmc.PagefileUsage;
		return 0;
	}
	return -1;
}

int ProcessState::getIoBytes(uint64_t* read_bytes, uint64_t* write_bytes)
{
	IO_COUNTERS io_counter;
	if (GetProcessIoCounters(GetCurrentProcess(), &io_counter))
	{
		if (read_bytes) *read_bytes = io_counter.ReadTransferCount;
		if (write_bytes) *write_bytes = io_counter.WriteTransferCount;
		return 0;
	}
	return -1;
}

void ProcessState::OnTimeOut()
{
	Qt::HANDLE id = QThread::currentThreadId();
	
	int cpu(0);
	uint64_t mem(0), vmem(0);
	
	getSysCpu(cpu);
	getMemoryUsage(&mem, &vmem);

	uint64_t memTemp = mem / 1024 / 1024;

	QString strTm = formatTime(m_time.elapsed());

	emit SigCpuMemUsageRate(cpu, (int)memTemp, strTm);

	if (m_isRunning == false) {
		m_pTimer->stop();
		qDebug() << "m_pTimer isactive = " << m_pTimer->isActive();
		m_pTimer->deleteLater();
		m_pTimer = nullptr;
	}
}

void ProcessState::Stop()
{
	m_isRunning = false;
	this->quit();
	while (this->isRunning()) {
		Sleep(5);
	}
}

void ProcessState::run()
{
	m_isRunning = true;
	Qt::HANDLE id = QThread::currentThreadId();
	m_pTimer = new QTimer;
	connect(m_pTimer, &QTimer::timeout, this, &ProcessState::OnTimeOut, Qt::DirectConnection);
	m_pTimer->start(1000);
	m_time.start();
	this->exec();
}

QString ProcessState::formatTime(int ms)
{
	int ss = 1000;
	int mi = ss * 60;
	int hh = mi * 60;
	int dd = hh * 24;

	long day = ms / dd;
	long hour = (ms - day * dd) / hh;
	long minute = (ms - day * dd - hour * hh) / mi;
	long second = (ms - day * dd - hour * hh - minute * mi) / ss;
	long milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

	QString hou = QString::number(hour, 10);
	QString min = QString::number(minute, 10);
	QString sec = QString::number(second, 10);
	QString msec = QString::number(milliSecond, 10);

	//qDebug() << "minute:" << min << "second" << sec << "ms" << msec <<endl;

	return hou + ":" + min + ":" + sec;
}