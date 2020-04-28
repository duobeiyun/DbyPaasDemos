#include "DbSettings.h"

DbSettings::DbSettings(QObject *parent)
	: QObject(parent)
{

}

DbSettings::~DbSettings()
{
}

void DbSettings::Write(const QString& key, const QString& value, const QString& org)
{
	QSettings settings("./config/settings.ini", QSettings::IniFormat);
	settings.beginGroup(org);
	settings.setValue(key, value);
	settings.endGroup();	
}

bool DbSettings::Read(const QString& key, QString& value, const QString& org)
{
	QSettings settings("./config/settings.ini", QSettings::IniFormat);
	//QTextCodec *codec = QTextCodec::codecForName("system");
	//configIniRead->setIniCodec(codec);

	QString strKey = org;
	strKey += "/";
	strKey += key;

	value = settings.value(strKey).toString();
	
	if (value.isEmpty())
		return false;
	return true;
}
