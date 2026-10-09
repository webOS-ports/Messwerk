#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDir>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString loggingPath READ getLoggingPath WRITE setLoggingPath NOTIFY loggingPathChanged)
    Q_PROPERTY(bool preventDisplayBlanking READ getPreventDisplayBlanking WRITE setPreventDisplayBlanking NOTIFY preventDisplayBlankingChanged)

    // The following property allows QML to check wether the Jolla Harbour version is currently running, which has less features due to banned libraries
    Q_PROPERTY(bool isHarbourVersion READ getIsHarbourVersion CONSTANT)
private:
    QSettings m_settings;

    Settings(); // singleton class, constructor is private

public:
    // Must first be called after the QCoreApplication exists
    static Settings& instance()
    {
        static Settings theSettings;
        return theSettings;
    }

    QString getLoggingPath() const { return m_settings.value("LoggingPath", QDir::home().absolutePath()).toString(); }
    bool getPreventDisplayBlanking() const { return m_settings.value("PreventDisplayBlanking", false).toBool(); }

    bool getIsHarbourVersion() const
    {
#ifdef FOR_HARBOUR
        return true;
#else
        return false;
#endif
    }

public slots:
    void setLoggingPath(const QString &path)
    {
        if(path == getLoggingPath()) {
            return;
        }

        m_settings.setValue("LoggingPath", path);
        emit loggingPathChanged(path);
    }

    void setPreventDisplayBlanking(bool prevent)
    {
        if(prevent == getPreventDisplayBlanking()) {
            return;
        }

        m_settings.setValue("PreventDisplayBlanking", prevent);
        emit preventDisplayBlankingChanged(prevent);
    }

signals:
    void loggingPathChanged(const QString &newPath);
    void preventDisplayBlankingChanged(bool prevented);
};

#endif // SETTINGS_H
