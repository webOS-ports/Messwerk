#ifndef LOGFILE_H
#define LOGFILE_H

#include <QFile>
#include <QString>

/*
 * Opens a new "<baseName>_<timestamp>.csv" in the configured logging
 * directory. The file is created exclusively (O_EXCL): an existing file - or
 * a symlink planted at that name in a shared directory - is never followed or
 * truncated. A name that is already taken, two logs started within the same
 * second for instance, gets a "-1", "-2", ... suffix instead.
 *
 * Returns false and fills errorString when no file could be created.
 */
bool openNewLogFile(QFile &file, const QString &baseName, QString *errorString);

#endif // LOGFILE_H
