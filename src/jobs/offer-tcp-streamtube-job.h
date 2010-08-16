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

#ifndef LIBKTELEPATHY_OFFER_TCP_STREAMTUBE_JOB_H
#define LIBKTELEPATHY_OFFER_TCP_STREAMTUBE_JOB_H

#include <kdemacros.h>

#include <telepathy-base-job.h>

#include <QVariantMap>
#include <TelepathyQt4/OutgoingStreamTubeChannel>

namespace Nepomuk {
    class PersonContact;
    class Person;
}

namespace Tp {
    class PendingOperation;
}

class QTcpServer;
class QTcpSocket;
class QHostAddress;

namespace KTelepathy {

class OfferTcpStreamTubeJobPrivate;
class KDE_EXPORT OfferTcpStreamTubeJob : public TelepathyBaseJob
{
    Q_OBJECT
    Q_DISABLE_COPY(OfferTcpStreamTubeJob)
    Q_DECLARE_PRIVATE(OfferTcpStreamTubeJob)

    enum ProcessingMode {
        OfferTcpStreamTubeContactMode,
        OfferTcpStreamTubeContactHostAddressMode,
        OfferTcpStreamTubeContactTcpServerMode,
        OfferTcpStreamTubeMetaContactMode,
        OfferTcpStreamTubeMetaContactHostAddressMode,
        OfferTcpStreamTubeMetaContactTcpServerMode
    };

    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__offerTcpStreamTubeContact())
    Q_PRIVATE_SLOT(d_func(), void __k__offerTcpStreamTubeContactHostAddress())
    Q_PRIVATE_SLOT(d_func(), void __k__offerTcpStreamTubeContactTcpServer())
    Q_PRIVATE_SLOT(d_func(), void __k__offerTcpStreamTubeMetaContact())
    Q_PRIVATE_SLOT(d_func(), void __k__offerTcpStreamTubeMetaContactHostAddress())
    Q_PRIVATE_SLOT(d_func(), void __k__offerTcpStreamTubeMetaContactTcpServer())

    Q_PRIVATE_SLOT(d_func(), void __k__onStreamTubeChannelCreated(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onStreamTubeChannelReady(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onOfferTubeFinished(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onStreamTubeChannelNewConnection(uint connectionId))
    Q_PRIVATE_SLOT(d_func(), void __k__onInvalidated())

    public:
        OfferTcpStreamTubeJob(const Nepomuk::PersonContact& contact, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        OfferTcpStreamTubeJob(const Nepomuk::PersonContact& contact, const QHostAddress& hostAddress, quint16 port = 0, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        OfferTcpStreamTubeJob(const Nepomuk::PersonContact& contact, QTcpServer* server, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        OfferTcpStreamTubeJob(const Nepomuk::Person& metacontact, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        OfferTcpStreamTubeJob(const Nepomuk::Person& metacontact, const QHostAddress& hostAddress, quint16 port = 0, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        OfferTcpStreamTubeJob(const Nepomuk::Person& metacontact, QTcpServer* server, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);

        virtual ~OfferTcpStreamTubeJob();

        virtual void start();

        QTcpServer* tcpServer();
        Tp::OutgoingStreamTubeChannelPtr outgoingStreamTubeChannel();
};

} // namespace KTelepathy

#endif // LIBKTELEPATHY_OFFER_TCP_STREAMTUBE_JOB_H
