#include <QDebug>

#include "activateable.h"

int Activateable::partIndex(unsigned part)
{
    switch(part) {
        case PART_PAGE:    return 0;
        case PART_COVER:   return 1;
        case PART_LOGGING: return 2;
        default:           return -1;
    }
}

void Activateable::activate(unsigned requestingPart)
{
    int idx = partIndex(requestingPart);

    if(idx < 0) {
        qWarning() << "Activateable: unknown part" << requestingPart;
        return;
    }

    m_partCount[idx]++;

    qDebug() << "Activated part" << requestingPart << "count" << m_partCount[idx];
}

void Activateable::deactivate(unsigned requestingPart)
{
    int idx = partIndex(requestingPart);

    if(idx < 0) {
        qWarning() << "Activateable: unknown part" << requestingPart;
        return;
    }

    // never wrap around: an unmatched deactivate must not leave the sensor
    // marked as wanted by four billion pages
    if(m_partCount[idx] == 0) {
        qWarning() << "Activateable: part" << requestingPart << "deactivated more often than activated";
        return;
    }

    m_partCount[idx]--;

    qDebug() << "Deactivated part" << requestingPart << "count" << m_partCount[idx];
}

bool Activateable::isActive() const
{
    for(unsigned count: m_partCount) {
        if(count != 0) {
            return true;
        }
    }

    return false;
}

bool Activateable::isPartActive(unsigned part) const
{
    int idx = partIndex(part);

    return idx >= 0 && m_partCount[idx] != 0;
}
