#pragma once

#include <QObject>
#include <QSettings>
class DbSettings : public QObject
{
	Q_OBJECT

public:
	DbSettings(QObject *parent);
	~DbSettings();

	static void Write(const QString& key, const QString& value, const QString& org = QString());
	static bool Read(const QString& key, QString& value, const QString& org = QString());
};
