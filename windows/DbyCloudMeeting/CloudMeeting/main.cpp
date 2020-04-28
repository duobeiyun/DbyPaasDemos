#include <QDir>
#include <QtWidgets/QApplication>
#include <QFileInfo>
#include "LoginDialog.h"
#include "easydump/easydump.h"
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <thread>

LONG WINAPI __UnhandledExceptionHandler(_EXCEPTION_POINTERS *pExceptionInfo);
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
	::SetUnhandledExceptionFilter(__UnhandledExceptionHandler);
	QApplication a(argc, argv);
	a.setAttribute(Qt::AA_UseDesktopOpenGL);
	qInstallMessageHandler(outputMessage);
	QFile styleFile("./Resources/style.qss");
	QFileInfo fileInfo("./Resources/style.qss");
	QString str = fileInfo.absolutePath();

	styleFile.open(QIODevice::ReadOnly);
	a.setStyleSheet(QString(styleFile.readAll()));

	LoginDialog loginDlg;
	loginDlg.show();
	return a.exec();
}

LONG WINAPI __UnhandledExceptionHandler(_EXCEPTION_POINTERS *pExceptionInfo)
{
	OutputDebugString(TEXT("Create a dump file since an exception occurred in sub-thread.\n"));
	int iRet = cpp4j::RecordExceptionInfo(pExceptionInfo, TEXT("CloudMeeting"));
	return iRet;
}

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	QMutexLocker locker(&mutex);
	QString text = "Debug:";
	bool bErrorFlag = false;
	bool bIsInfo = false;
	switch (type)
	{
	case QtInfoMsg:
		text = QString("---Info: ");
		bIsInfo = true;
		break;
	case QtDebugMsg:
		text = QString("---Debug: ");
		break;
	case QtWarningMsg:
		text = QString("###Warning: ");
		break;
	case QtCriticalMsg:
		text = QString("***Critical: ");
		bErrorFlag = true;
		break;
	case QtFatalMsg:
		text = QString("@@@Fatal: ");
		bErrorFlag = true;
		break;
	}

	QString context_info = QString("%1 %2 ").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(current_date).arg(msg);

	QString strErrorFile, strInfoFile;
	QString str = QCoreApplication::applicationDirPath();
	str.append("/log");
	
	QDir dir;
	if (dir.mkpath(str) == false)
		return;

	{
		QString current_date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
		strErrorFile = str;
		strInfoFile = str;
		str.append("/").append(current_date).append(".txt");
		strErrorFile.append("/").append("error_").append(current_date).append(".txt");
		strInfoFile.append("/").append("info_").append(current_date).append(".txt");
	}

	str.replace("/", "\\");
	strErrorFile.replace("/", "\\");
	strInfoFile.replace("/", "\\");
	QFile file(str);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();
	if (bErrorFlag) {
		QFile errorFile(strErrorFile);
		errorFile.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream text_stream_error(&errorFile);
		text_stream_error << message << "\r\n";
		errorFile.flush();
		errorFile.close();
	}

	if (bIsInfo) {
		QFile infoFile(strInfoFile);
		infoFile.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream text_stream_error(&infoFile);
		text_stream_error << message << "\r\n";
		infoFile.flush();
		infoFile.close();
	}
}