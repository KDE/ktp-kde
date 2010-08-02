/*
* Copyright (C) 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef LIBKTELEPATHY_OFFER_DBUSTUBE_JOB_H
#define LIBKTELEPATHY_OFFER_DBUSTUBE_JOB_H

#include <kdemacros.h>

#include "telepathy-base-job.h"

#include <QVariantMap>
#include <TelepathyQt4/OutgoingDBusTubeChannel>
#include <TelepathyQt4/Contact>

class QDBusConnection;
namespace Nepomuk {
    class PersonContact;
    class Person;
}

namespace Tp {
    class PendingOperation;
}

class QDBusConnection;

class OfferDBusTubeJobPrivate;
class KDE_EXPORT OfferDBusTubeJob : public TelepathyBaseJob
{
    Q_OBJECT
    Q_DISABLE_COPY(OfferDBusTubeJob)
    Q_DECLARE_PRIVATE(OfferDBusTubeJob)

    enum ProcessingMode {
        OfferDBusTubeContactMode,
//TODO?        OfferDBusTubeContactServerMode,
        OfferDBusTubeMetaContactMode,
//TODO?        OfferDBusTubeMetaContactServerMode
    };

    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__offerDBusTubeContact())
//TODO?    Q_PRIVATE_SLOT(d_func(), void __k__offerDBusTubeContactServer())
    Q_PRIVATE_SLOT(d_func(), void __k__offerDBusTubeMetaContact())
//TODO?    Q_PRIVATE_SLOT(d_func(), void __k__offerDBusTubeMetaContactServer())

    Q_PRIVATE_SLOT(d_func(), void __k__onDBusTubeChannelCreated(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onDBusTubeChannelReady(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onOfferTubeFinished(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onBusNamesChanged(const QHash< Tp::ContactPtr, QString >& added, const QList< Tp::ContactPtr >& removed ))
    Q_PRIVATE_SLOT(d_func(), void __k__onInvalidated())

    public:
        OfferDBusTubeJob(const Nepomuk::PersonContact& contact,
                         const QLatin1String serviceName = QLatin1String("org.kde.KTelepathy.DefaultServiceName"),
                         const QVariantMap& parameters = QVariantMap(),
                         QObject* parent = 0);
        OfferDBusTubeJob(const Nepomuk::Person& metacontact,
                         const QLatin1String serviceName = QLatin1String("org.kde.KTelepathy.DefaultServiceName"),
                         const QVariantMap& parameters = QVariantMap(),
                         QObject* parent = 0);

        virtual ~OfferDBusTubeJob();

        virtual void start();

        QDBusConnection dbusConnection();
        Tp::OutgoingDBusTubeChannelPtr outgoingDBusTubeChannel();
};

#endif // LIBKTELEPATHY_OFFER_DBUSTUBE_JOB_H
