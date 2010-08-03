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

#include "offer-tcp-streamtube-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"

#include <person.h>
#include <personcontact.h>
#include <imaccount.h>

#include <QTimer>
#include <QTcpServer>
#include <QHostAddress>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Feature>
#include <TelepathyQt4/StreamTubeChannel>
#include <TelepathyQt4/OutgoingStreamTubeChannel>
#include <TelepathyQt4/ConnectionCapabilities>


class OfferTcpStreamTubeJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(OfferTcpStreamTubeJob)

    public:
        OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeJob::ProcessingMode m, const QVariantMap& p, const QHostAddress& h);
        virtual ~OfferTcpStreamTubeJobPrivate();

        OfferTcpStreamTubeJob::ProcessingMode mode;

        Tp::OutgoingStreamTubeChannelPtr channel;
        Nepomuk::PersonContact contact;
        Nepomuk::Person metacontact;

        const QVariantMap parameters;
        const QHostAddress hostAddress;
        quint16 port;
        QTcpServer* server;

        // Operation Q_PRIVATE_SLOTS
        void __k__offerTcpStreamTubeContact();
        void __k__offerTcpStreamTubeContactHostAddress();
        void __k__offerTcpStreamTubeContactTcpServer();
        void __k__offerTcpStreamTubeMetaContact();
        void __k__offerTcpStreamTubeMetaContactHostAddress();
        void __k__offerTcpStreamTubeMetaContactTcpServer();

        void __k__onStreamTubeChannelCreated(Tp::PendingOperation* op);
        void __k__onStreamTubeChannelReady(Tp::PendingOperation* op);
        void __k__onOfferTubeFinished(Tp::PendingOperation* op);
        void __k__onStreamTubeChannelNewConnection(uint connectionId);
        void __k__onInvalidated();

        bool findBestContactFromMetaContact();
};

OfferTcpStreamTubeJob::OfferTcpStreamTubeJob(const Nepomuk::PersonContact& contact,
                                             const QVariantMap& parameters,
                                             QObject* parent)
    : TelepathyBaseJob(*new OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeContactMode, parameters, *new QHostAddress()), parent)
{
    Q_D(OfferTcpStreamTubeJob);

    d->contact = contact;
    d->server = new QTcpServer(parent);
}


OfferTcpStreamTubeJob::OfferTcpStreamTubeJob(const Nepomuk::PersonContact& contact,
                                             const QHostAddress& hostAddress,
                                             quint16 port,
                                             const QVariantMap& parameters,
                                             QObject* parent)
    : TelepathyBaseJob(*new OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeContactHostAddressMode, parameters, hostAddress), parent)
{
    Q_D(OfferTcpStreamTubeJob);

    d->contact = contact;
    d->port = port;
    d->server = new QTcpServer(parent);
    d->server->listen(hostAddress, port);

}


OfferTcpStreamTubeJob::OfferTcpStreamTubeJob(const Nepomuk::PersonContact& contact,
                                             QTcpServer* server,
                                             const QVariantMap& parameters,
                                             QObject* parent)
    : TelepathyBaseJob(*new OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeContactTcpServerMode, parameters, *new QHostAddress()), parent)

{
    Q_D(OfferTcpStreamTubeJob);

    d->contact = contact;
    if (!server->isListening())
        kWarning() << "Server is not listening"; //TODO
    d->server = server;
}


OfferTcpStreamTubeJob::OfferTcpStreamTubeJob(const Nepomuk::Person& metacontact,
                                             const QVariantMap& parameters,
                                             QObject* parent)
    : TelepathyBaseJob(*new OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeMetaContactMode, parameters, *new QHostAddress()), parent)
{
    Q_D(OfferTcpStreamTubeJob);

    d->metacontact = metacontact;
    d->server = new QTcpServer(parent);
    d->server->listen();
}


OfferTcpStreamTubeJob::OfferTcpStreamTubeJob(const Nepomuk::Person& metacontact,
                                             const QHostAddress& hostAddress,
                                             quint16 port,
                                             const QVariantMap& parameters,
                                             QObject* parent)
    : TelepathyBaseJob(*new OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeMetaContactHostAddressMode, parameters, hostAddress), parent)
{
    Q_D(OfferTcpStreamTubeJob);

    d->metacontact = metacontact;
    d->port = port;
    d->server = new QTcpServer(parent);
    d->server->listen(hostAddress, port);
}




OfferTcpStreamTubeJob::OfferTcpStreamTubeJob(const Nepomuk::Person& metacontact,
                                             QTcpServer* server,
                                             const QVariantMap& parameters,
                                             QObject* parent)
    : TelepathyBaseJob(*new OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeMetaContactTcpServerMode, parameters, *new QHostAddress()), parent)
{
    Q_D(OfferTcpStreamTubeJob);

    d->metacontact = metacontact;
    d->server = server;
}


OfferTcpStreamTubeJob::~OfferTcpStreamTubeJob()
{
    kDebug();
}


void OfferTcpStreamTubeJob::start()
{
    Q_D(OfferTcpStreamTubeJob);

    // What are we supposed to do?
    switch (d->mode) {
        case OfferTcpStreamTubeContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerTcpStreamTubeContact()));
            break;
        case OfferTcpStreamTubeContactHostAddressMode:
            QTimer::singleShot(0, this, SLOT(__k__offerTcpStreamTubeContactHostAddress()));
            break;
        case OfferTcpStreamTubeContactTcpServerMode:
            QTimer::singleShot(0, this, SLOT(__k__offerTcpStreamTubeContactTcpServer()));
            break;
        case OfferTcpStreamTubeMetaContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerTcpStreamTubeMetaContact()));
            break;
        case OfferTcpStreamTubeMetaContactHostAddressMode:
            QTimer::singleShot(0, this, SLOT(__k__offerTcpStreamTubeMetaContactHostAddress()));
            break;
        case OfferTcpStreamTubeMetaContactTcpServerMode:
            QTimer::singleShot(0, this, SLOT(__k__offerTcpStreamTubeMetaContactTcpServer()));
            break;
        default:
            // Hmm?
            setError(TelepathyBridge::InvalidOperationError);
            setErrorText(i18n("This is an internal error of KTelepathy"));
            QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
            break;
    }
}


QTcpServer* OfferTcpStreamTubeJob::tcpServer()
{
    Q_D(OfferTcpStreamTubeJob);
    return d->server;
}


Tp::OutgoingStreamTubeChannelPtr OfferTcpStreamTubeJob::outgoingStreamTubeChannel()
{
    Q_D(OfferTcpStreamTubeJob);
    return d->channel;
}


OfferTcpStreamTubeJobPrivate::OfferTcpStreamTubeJobPrivate(OfferTcpStreamTubeJob::ProcessingMode m, const QVariantMap& p, const QHostAddress& h)
    : mode(m),
      parameters(p),
      hostAddress(h),
      server(NULL) {}


OfferTcpStreamTubeJobPrivate::~OfferTcpStreamTubeJobPrivate() {}


void OfferTcpStreamTubeJobPrivate::__k__offerTcpStreamTubeContact()
{
    // TODO here or in constructor
    // is this really needed?
    server->listen();

    __k__offerTcpStreamTubeContactTcpServer();
}


void OfferTcpStreamTubeJobPrivate::__k__offerTcpStreamTubeContactHostAddress()
{
    // TODO here or in constructor
    // is this really needed?
    server->listen(hostAddress, port);

    // TODO use
    //   PendingOperation *offerTcpSocket(const QHostAddress &address, quint16 port, const QVariantMap &parameters);
    // instead of
    //   PendingOperation *offerTcpSocket(QTcpServer *server, const QVariantMap &parameters);

    __k__offerTcpStreamTubeContactTcpServer();
}


void OfferTcpStreamTubeJobPrivate::__k__offerTcpStreamTubeContactTcpServer()
{
    Q_Q(OfferTcpStreamTubeJob);

    QHash< TelepathyAccountProxy*, QList< Tp::ContactPtr > > tbd;
    QHash< TelepathyAccountProxy*, QList< Nepomuk::PersonContact > > proxyToContacts;

    QStringList imIDs;

    foreach (const Nepomuk::IMAccount& account, contact.iMAccounts()) {
        imIDs << account.imIDs();
    }

    foreach (TelepathyAccountProxy* proxy, TelepathyBridge::instance()->d_func()->accountProxiesForContact(contact)) {
        tbd[proxy] << proxy->contactsForIdentifiers(imIDs);
        proxyToContacts[proxy] << contact;
    }

    if (tbd.isEmpty()) {
        kWarning() << "No contacts retrieved!";
        // Give it up and emit an error
        q->setError(TelepathyBridge::NoContactsFoundError);
        q->setErrorText(i18n("No existing Telepathy contacts could be mapped to the chosen contact"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

//TODO drdanz check if target contact is online?

    QHash< TelepathyAccountProxy*, QList< Tp::ContactPtr > >::const_iterator i;
    for (i = tbd.constBegin(); i != tbd.constEnd(); ++i) {
        // Can we do it?
        //TODO drdanz: supportsStreamTubes() not yet supported in TpQt4
        //if (i.key()->account()->connection()->capabilities()->supportsStreamTubes()) {
        if (1) {
            // Can target contact accept it?
            //TODO drdanz
            if (1) {
                uint contactHandle = i.value().first()->handle()[0];

                QVariantMap request;
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"), QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_STREAM_TUBE));
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"), (uint) Tp::HandleTypeContact);
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"), contactHandle);
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_STREAM_TUBE ".Service"), QLatin1String("rsync"));
                kDebug() << i18n("Request:") << request;

                Tp::PendingChannel* pndChan = i.key()->account()->connection()->createChannel(request);
                q->connect(pndChan,
                           SIGNAL(finished(Tp::PendingOperation*)),
                           SLOT(__k__onStreamTubeChannelCreated(Tp::PendingOperation*)));
                return;
            }
        }
    }
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferTcpStreamTubeJobPrivate::__k__offerTcpStreamTubeMetaContact()
{
    if(findBestContactFromMetaContact()) {
        __k__offerTcpStreamTubeContact();
        return;
    }

    Q_Q(OfferTcpStreamTubeJob);
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferTcpStreamTubeJobPrivate::__k__offerTcpStreamTubeMetaContactHostAddress()
{
    if(findBestContactFromMetaContact()) {
        __k__offerTcpStreamTubeContactHostAddress();
        return;
    }

    Q_Q(OfferTcpStreamTubeJob);
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferTcpStreamTubeJobPrivate::__k__offerTcpStreamTubeMetaContactTcpServer()
{
    if(findBestContactFromMetaContact()) {
        __k__offerTcpStreamTubeContactTcpServer();
        return;
    }

    Q_Q(OfferTcpStreamTubeJob);
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferTcpStreamTubeJobPrivate::__k__onStreamTubeChannelCreated(Tp::PendingOperation* op)
{
    Q_Q(OfferTcpStreamTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to create stream tube channel") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    kDebug() << i18n("Stream tube channel created!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel created!"));

    Tp::PendingChannel* pc = qobject_cast<Tp::PendingChannel*>(op);
    channel = Tp::OutgoingStreamTubeChannelPtr::dynamicCast(pc->channel());

    kDebug() << channel.data();

    q->connect(channel.data(),
            SIGNAL(invalidated(Tp::DBusProxy*, const QString&, const QString&)),
            SLOT(__k__onInvalidated()));
    Tp::Features features = Tp::Features() << Tp::TubeChannel::FeatureTube
                                           << Tp::StreamTubeChannel::FeatureStreamTube
                                           << Tp::StreamTubeChannel::FeatureConnectionMonitoring;
    q->connect(channel->becomeReady(features),
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onStreamTubeChannelReady(Tp::PendingOperation*)));
}


void OfferTcpStreamTubeJobPrivate::__k__onStreamTubeChannelReady(Tp::PendingOperation* op)
{
    Q_Q(OfferTcpStreamTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to make stream tube channel ready") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    kDebug() << i18n("Stream tube channel ready!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel ready!"));

    q->connect(channel.data(),
            SIGNAL(newConnection(uint)),
            SLOT(__k__onStreamTubeChannelNewConnection(uint)));

    operations.clear();
    Tp::PendingOperation* offerTubeOperation = channel->offerTcpSocket(server, parameters);
    q->connect(offerTubeOperation, SIGNAL(finished(Tp::PendingOperation*)),
               q, SLOT(__k__onOfferTubeFinished(Tp::PendingOperation*)));
    addOperation(offerTubeOperation);
}


void OfferTcpStreamTubeJobPrivate::__k__onOfferTubeFinished(Tp::PendingOperation* op)
{
    Q_Q(OfferTcpStreamTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to open stream tube channel") <<"-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    kDebug() << i18n("Stream tube channel opened!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel opened!"));
}


void OfferTcpStreamTubeJobPrivate::__k__onStreamTubeChannelNewConnection(uint connectionId)
{
    Q_Q(OfferTcpStreamTubeJob);

    kDebug() << i18n("New remote connection with ID") << connectionId;
    kDebug() << channel->connectionsForSourceAddresses();
    kDebug() << channel->contactsForConnections();
    Q_EMIT q->infoMessage(q, i18n("New remote connection!"));
}


void OfferTcpStreamTubeJobPrivate::__k__onInvalidated()
{
    Q_Q(OfferTcpStreamTubeJob);

    kWarning() << i18n("Stream tube invalidated!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube invalidated!"));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


bool OfferTcpStreamTubeJobPrivate::findBestContactFromMetaContact()
{
    foreach(Nepomuk::PersonContact foundContact, TelepathyBridge::instance()->d_func()->contactsForMetaContact(metacontact)) {
        // TODO Can we do it?
        if (1) {
            // TODO Can target contact accept it?
            if (1) {
                contact = foundContact;
                return true;
            }
        }
    }
    return false;
}

#include "offer-tcp-streamtube-job.moc"
