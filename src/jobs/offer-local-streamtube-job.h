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

#ifndef LIBKTELEPATHY_OFFER_LOCAL_STREAMTUBE_JOB_H
#define LIBKTELEPATHY_OFFER_LOCAL_STREAMTUBE_JOB_H

#include <kdemacros.h>

#include "telepathy-base-job.h"

#include <QVariantMap>
#include <TelepathyQt4/OutgoingStreamTubeChannel>

namespace Nepomuk {
    class PersonContact;
    class Person;
}

namespace Tp {
    class PendingOperation;
}

class QLocalServer;

class OfferLocalStreamTubeJobPrivate;
class KDE_EXPORT OfferLocalStreamTubeJob : public TelepathyBaseJob
{
    Q_OBJECT
    Q_DISABLE_COPY(OfferLocalStreamTubeJob)
    Q_DECLARE_PRIVATE(OfferLocalStreamTubeJob)

    enum ProcessingMode {
        OfferLocalStreamTubeContactMode,
        OfferLocalStreamTubeContactSocketAddressMode,
        OfferLocalStreamTubeContactLocalServerMode,
        OfferLocalStreamTubeMetaContactMode,
        OfferLocalStreamTubeMetaContactSocketAddressMode,
        OfferLocalStreamTubeMetaContactLocalServerMode
    };

    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__offerLocalStreamTubeContact())
    Q_PRIVATE_SLOT(d_func(), void __k__offerLocalStreamTubeContactSocketAddress())
    Q_PRIVATE_SLOT(d_func(), void __k__offerLocalStreamTubeContactLocalServer())
    Q_PRIVATE_SLOT(d_func(), void __k__offerLocalStreamTubeMetaContact())
    Q_PRIVATE_SLOT(d_func(), void __k__offerLocalStreamTubeMetaContactSocketAddress())
    Q_PRIVATE_SLOT(d_func(), void __k__offerLocalStreamTubeMetaContactLocalServer())

    Q_PRIVATE_SLOT(d_func(), void __k__onStreamTubeChannelCreated(Tp::PendingOperation *op))
    Q_PRIVATE_SLOT(d_func(), void __k__onStreamTubeChannelReady(Tp::PendingOperation *op))
    Q_PRIVATE_SLOT(d_func(), void __k__onOfferTubeFinished(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onStreamTubeChannelNewConnection(uint connectionId))
    Q_PRIVATE_SLOT(d_func(), void __k__onInvalidated())

    public:
        OfferLocalStreamTubeJob(const Nepomuk::PersonContact& contact, bool requireCredentials = false, const QVariantMap& parameters = *new QVariantMap(), QObject* parent = 0);
        OfferLocalStreamTubeJob(const Nepomuk::PersonContact& contact, const QString& socketAddress, bool requireCredentials = false, const QVariantMap& parameters = *new QVariantMap(), QObject* parent = 0);
        OfferLocalStreamTubeJob(const Nepomuk::PersonContact& contact, QLocalServer* server, bool requireCredentials = false, const QVariantMap& parameters = *new QVariantMap(), QObject* parent = 0);
        OfferLocalStreamTubeJob(const Nepomuk::Person& metacontact, bool requireCredentials = false, const QVariantMap& parameters = *new QVariantMap(), QObject* parent = 0);
        OfferLocalStreamTubeJob(const Nepomuk::Person& metacontact, const QString& socketAddress, bool requireCredentials = false, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        OfferLocalStreamTubeJob(const Nepomuk::Person& metacontact, QLocalServer* server, bool requireCredentials = false, const QVariantMap& parameters = *new QVariantMap(), QObject* parent = 0);

        virtual ~OfferLocalStreamTubeJob();

        virtual void start();

        QLocalServer* localServer();
        Tp::OutgoingStreamTubeChannelPtr outgoingStreamTubeChannel();
};

#endif // LIBKTELEPATHY_OFFER_LOCAL_STREAMTUBE_JOB_H
