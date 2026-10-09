#include <QDateTime>
#include <QDebug>
#include <QDir>

#include "settings.h"

#include "logfile.h"

bool openNewLogFile(QFile &file, const QString &baseName, QString *errorString)
{
    static const int MAX_ATTEMPTS = 100;

    QDir dir(Settings::instance().getLoggingPath());

    if(!dir.mkpath(QStringLiteral("."))) {
        if(errorString) {
            *errorString = QStringLiteral("Cannot create the folder %1").arg(dir.absolutePath());
        }
        return false;
    }

    QString stem = baseName + QLatin1Char('_') +
                   QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-hhmmss"));

    for(int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        QString fileName = stem;

        if(attempt > 0) {
            fileName += QLatin1Char('-') + QString::number(attempt);
        }

        file.setFileName(dir.filePath(fileName + QStringLiteral(".csv")));

        // NewOnly is the exclusive create: no check-then-open window
        if(file.open(QIODevice::WriteOnly | QIODevice::NewOnly | QIODevice::Text)) {
            qDebug() << "Logging to" << file.fileName();
            return true;
        }

        if(!file.exists()) {
            // failed for another reason than the name being taken
            break;
        }
    }

    if(errorString) {
        *errorString = QStringLiteral("Cannot create %1: %2").arg(file.fileName(), file.errorString());
    }

    return false;
}
