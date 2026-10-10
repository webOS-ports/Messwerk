#include "settings.h"

Settings::Settings()
    : QObject(), m_settings(QStringLiteral("bytewerk"), QStringLiteral("Messwerk"))
{
}
