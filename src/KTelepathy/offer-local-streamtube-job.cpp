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

#include "offer-local-streamtube-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"

#include <person.h>
#include <personcontact.h>
#include <imaccount.h>

#include <QTimer>
#include <QLocalServer>

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


class OfferLocalStreamTubeJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(OfferLocalStreamTubeJob)

    public:
        OfferLocalStreamTubeJobPrivate(OfferLocalStreamTubeJob* parent, OfferLocalStreamTubeJob::ProcessingMode m, const QVariantMap& p, const QByteArray& s);
        virtual ~OfferLocalStreamTubeJobPrivate();

        OfferLocalStreamTubeJob::ProcessingMode mode;

        Tp::OutgoingStreamTubeChannelPtr channel;
        Nepomuk::PersonContact contact;
        Nepomuk::Person metacontact;

        bool requireCredentials;
        const QVariantMap& parameters;
        const QByteArray& socketAddress;
        QLocalServer* server;

        // Operation Q_PRIVATE_SLOTS
        void __k__offerLocalStreamTubeContact();
        void __k__offerLocalStreamTubeContactSocketAddress();
        void __k__offerLocalStreamTubeContactLocalServer();
        void __k__offerLocalStreamTubeMetaContact();
        void __k__offerLocalStreamTubeMetaContactSocketAddress();
        void __k__offerLocalStreamTubeMetaContactLocalServer();

        void __k__onStreamTubeChannelCreated(Tp::PendingOperation* op);
        void __k__onStreamTubeChannelReady(Tp::PendingOperation* op);
        void __k__onOfferTubeFinished(Tp::PendingOperation* op);
        void __k__onStreamTubeChannelNewConnection(uint connectionId);
        void __k__onInvalidated();

        bool findBestContactFromMetaContact();
};

OfferLocalStreamTubeJob::OfferLocalStreamTubeJob(const Nepomuk::PersonContact& contact,
                                                 bool requireCredentials,
                                                 const QVariantMap& parameters,
                                                 QObject* parent)
    : TelepathyBaseJob(*new OfferLocalStreamTubeJobPrivate(this, OfferLocalStreamTubeContactMode, parameters, *new QByteArray()), parent)
{
    Q_D(OfferLocalStreamTubeJob);

    d->contact = contact;
    d->requireCredentials = requireCredentials;
    d->server = new QLocalServer(parent);
//    d->server->listen();
}


OfferLocalStreamTubeJob::OfferLocalStreamTubeJob(const Nepomuk::PersonContact& contact,
                                                 const QByteArray& socketAddress,
                                                 bool requireCredentials,
                                                 const QVariantMap& parameters,
                                                 QObject* parent)
    : TelepathyBaseJob(*new OfferLocalStreamTubeJobPrivate(this, OfferLocalStreamTubeContactSocketAddressMode, parameters, socketAddress), parent)

{
    Q_D(OfferLocalStreamTubeJob);

    d->contact = contact;
    d->requireCredentials = requireCredentials;
    d->server = new QLocalServer(parent);
    d->server->listen(socketAddress);
}


OfferLocalStreamTubeJob::OfferLocalStreamTubeJob(const Nepomuk::PersonContact& contact,
                                                 QLocalServer* server,
                                                 bool requireCredentials,
                                                 const QVariantMap& parameters,
                                                 QObject* parent)
    : TelepathyBaseJob(*new OfferLocalStreamTubeJobPrivate(this, OfferLocalStreamTubeContactLocalServerMode, parameters, *new QByteArray()), parent)

{
    Q_D(OfferLocalStreamTubeJob);

    d->contact = contact;
    d->requireCredentials = requireCredentials;
    if (!server->isListening())
        kWarning() << "Server is not listening"; //TODO
    d->server = server;

}


OfferLocalStreamTubeJob::OfferLocalStreamTubeJob(const Nepomuk::Person& metacontact,
                                                 bool requireCredentials,
                                                 const QVariantMap& parameters,
                                                 QObject* parent)
    : TelepathyBaseJob(*new OfferLocalStreamTubeJobPrivate(this, OfferLocalStreamTubeMetaContactMode, parameters, *new QByteArray()), parent)
{
    Q_D(OfferLocalStreamTubeJob);

    d->metacontact = metacontact;
    d->requireCredentials = requireCredentials;
    d->server = new QLocalServer(parent);
//    d->server->listen();
}


OfferLocalStreamTubeJob::OfferLocalStreamTubeJob(const Nepomuk::Person& metacontact,
                                                 const QByteArray& socketAddress,
                                                 bool requireCredentials,
                                                 const QVariantMap& parameters,
                                                 QObject* parent)
    : TelepathyBaseJob(*new OfferLocalStreamTubeJobPrivate(this, OfferLocalStreamTubeContactSocketAddressMode, parameters, socketAddress), parent)
{
    Q_D(OfferLocalStreamTubeJob);

    d->metacontact = metacontact;
    d->requireCredentials = requireCredentials;
    d->server = new QLocalServer(parent);
    d->server->listen(socketAddress);
}


OfferLocalStreamTubeJob::OfferLocalStreamTubeJob(const Nepomuk::Person& metacontact,
                                                 QLocalServer* server,
                                                 bool requireCredentials,
                                                 const QVariantMap& parameters,
                                                 QObject* parent)
    : TelepathyBaseJob(*new OfferLocalStreamTubeJobPrivate(this, OfferLocalStreamTubeContactLocalServerMode, parameters, *new QByteArray()), parent)
{
    Q_D(OfferLocalStreamTubeJob);

    d->metacontact = metacontact;
    d->server = server;
    d->requireCredentials = requireCredentials;
    if (!server->isListening())
        kWarning() << "Server is not listening"; //TODO
    d->server = server;
}


OfferLocalStreamTubeJob::~OfferLocalStreamTubeJob()
{
    kDebug();
}


void OfferLocalStreamTubeJob::start()
{
    Q_D(OfferLocalStreamTubeJob);

    // What are we supposed to do?
    switch (d->mode) {
        case OfferLocalStreamTubeContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerLocalStreamTubeContact()));
            break;
        case OfferLocalStreamTubeContactSocketAddressMode:
            QTimer::singleShot(0, this, SLOT(__k__offerLocalStreamTubeContactSocketAddress()));
            break;
        case OfferLocalStreamTubeContactLocalServerMode:
            QTimer::singleShot(0, this, SLOT(__k__offerLocalStreamTubeContactLocalServer()));
            break;
        case OfferLocalStreamTubeMetaContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerLocalStreamTubeMetaContact()));
            break;
        case OfferLocalStreamTubeMetaContactSocketAddressMode:
            QTimer::singleShot(0, this, SLOT(__k__offerLocalStreamTubeMetaContactSocketAddress()));
            break;
        case OfferLocalStreamTubeMetaContactLocalServerMode:
            QTimer::singleShot(0, this, SLOT(__k__offerLocalStreamTubeMetaContactLocalServer()));
            break;
        default:
            // Hmm?
            setError(TelepathyBridge::InvalidOperationError);
            setErrorText(i18n("This is an internal error of KTelepathy"));
            QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
            break;
    }
}


QLocalServer* OfferLocalStreamTubeJob::localServer()
{
    Q_D(OfferLocalStreamTubeJob);
    return d->server;
}


Tp::OutgoingStreamTubeChannelPtr OfferLocalStreamTubeJob::outgoingStreamTubeChannel()
{
    Q_D(OfferLocalStreamTubeJob);
    return d->channel;
}


OfferLocalStreamTubeJobPrivate::OfferLocalStreamTubeJobPrivate(OfferLocalStreamTubeJob* parent, OfferLocalStreamTubeJob::ProcessingMode m, const QVariantMap& p, const QByteArray& s)
    : TelepathyBaseJobPrivate(parent),
      mode(m),
      parameters(p),
      socketAddress(s),
      server(NULL) {}


OfferLocalStreamTubeJobPrivate::~OfferLocalStreamTubeJobPrivate()
{}


void OfferLocalStreamTubeJobPrivate::__k__offerLocalStreamTubeContact()
{
    // TODO here or in constructor
    // is this really needed?
//    server->listen();

    __k__offerLocalStreamTubeContactLocalServer();
}


void OfferLocalStreamTubeJobPrivate::__k__offerLocalStreamTubeContactSocketAddress()
{
    // TODO here or in constructor
    // is this really needed?
    server->listen(socketAddress);

    __k__offerLocalStreamTubeContactLocalServer();
}


void OfferLocalStreamTubeJobPrivate::__k__offerLocalStreamTubeContactLocalServer()
{
    Q_Q(OfferLocalStreamTubeJob);

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


void OfferLocalStreamTubeJobPrivate::__k__offerLocalStreamTubeMetaContact()
{
    if(findBestContactFromMetaContact()) {
        __k__offerLocalStreamTubeContact();
        return;
    }

    Q_Q(OfferLocalStreamTubeJob);
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferLocalStreamTubeJobPrivate::__k__offerLocalStreamTubeMetaContactSocketAddress()
{
    if(findBestContactFromMetaContact()) {
        __k__offerLocalStreamTubeContactSocketAddress();
        return;
    }

    Q_Q(OfferLocalStreamTubeJob);
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferLocalStreamTubeJobPrivate::__k__offerLocalStreamTubeMetaContactLocalServer()
{
    if(findBestContactFromMetaContact()) {
        __k__offerLocalStreamTubeContactLocalServer();
        return;
    }

    Q_Q(OfferLocalStreamTubeJob);
    //TODO error streamtube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferLocalStreamTubeJobPrivate::__k__onStreamTubeChannelCreated(Tp::PendingOperation* op)
{
    Q_Q(OfferLocalStreamTubeJob);

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


void OfferLocalStreamTubeJobPrivate::__k__onStreamTubeChannelReady(Tp::PendingOperation* op)
{
    Q_Q(OfferLocalStreamTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to make stream tube channel ready") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    kDebug() << i18n("Stream tube channel ready!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel ready!"));

    q->connect(channel.data(),
            SIGNAL(newConnection(Tp::ContactPtr,QVariant,uint)),
            SLOT(__k__onStreamTubeChannelNewConnection(Tp::ContactPtr,QVariant,uint)));

    operations.clear();
    Tp::PendingOperation* offerTubeOperation = channel->offerUnixSocket(server, QVariantMap(), requireCredentials);
    q->connect(offerTubeOperation, SIGNAL(finished(Tp::PendingOperation*)),
               q, SLOT(__k__onOfferTubeFinished(Tp::PendingOperation*)));
    addOperation(offerTubeOperation);
}


void OfferLocalStreamTubeJobPrivate::__k__onOfferTubeFinished(Tp::PendingOperation* op)
{
    Q_Q(OfferLocalStreamTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to open stream tube channel") <<"-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    kDebug() << i18n("Stream tube channel opened!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel opened!"));
}


void OfferLocalStreamTubeJobPrivate::__k__onStreamTubeChannelNewConnection(uint connectionId)
{
    Q_Q(OfferLocalStreamTubeJob);

    kDebug() << i18n("New remote connection with ID") << connectionId;
    kDebug() << channel->connectionsForSourceAddresses();
    kDebug() << channel->contactsForConnections();
    Q_EMIT q->infoMessage(q, i18n("New remote connection!"));
}


void OfferLocalStreamTubeJobPrivate::__k__onInvalidated()
{
    Q_Q(OfferLocalStreamTubeJob);

    kWarning() << i18n("Stream tube invalidated!");
    Q_EMIT q->infoMessage(q, i18n("Stream tube invalidated!"));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


bool OfferLocalStreamTubeJobPrivate::findBestContactFromMetaContact()
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

#include "offer-local-streamtube-job.moc"
