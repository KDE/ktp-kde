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

#include "offer-dbustube-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"

#include "ontologies/person.h"
#include "ontologies/personcontact.h"
#include "ontologies/imaccount.h"

#include <QTimer>
#include <QDBusConnection>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Contact>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Feature>
#include <TelepathyQt4/DBusTubeChannel>
#include <TelepathyQt4/OutgoingDBusTubeChannel>
#include <TelepathyQt4/ConnectionCapabilities>
#include <TelepathyQt4/Types>


class OfferDBusTubeJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(OfferDBusTubeJob)

    public:
        OfferDBusTubeJobPrivate(const Nepomuk::PersonContact& contact,
                                const QString serviceName,
                                const QVariantMap& p);
        OfferDBusTubeJobPrivate(const Nepomuk::Person& metacontact,
                                const QString serviceName,
                                const QVariantMap& p);
        virtual ~OfferDBusTubeJobPrivate();

        OfferDBusTubeJob::ProcessingMode mode;

        Tp::OutgoingDBusTubeChannelPtr channel;
        Nepomuk::PersonContact contact;
        Nepomuk::Person metacontact;
        QString serviceName;

        const QVariantMap parameters;
        QDBusConnection connection;

        // Operation Q_PRIVATE_SLOTS
        void __k__offerDBusTubeContact();
//TODO?        void __k__offerDBusTubeContactServer();
        void __k__offerDBusTubeMetaContact();
//TODO?        void __k__offerDBusTubeMetaContactServer();

        void __k__onDBusTubeChannelCreated(Tp::PendingOperation* op);
        void __k__onDBusTubeChannelReady(Tp::PendingOperation* op);
        void __k__onOfferTubeFinished(Tp::PendingOperation* op);
        void __k__onBusNamesChanged(const QHash< Tp::ContactPtr, QString >& added, const QList< Tp::ContactPtr >& removed );
        void __k__onInvalidated();

        bool findBestContactFromMetaContact();
};

OfferDBusTubeJob::OfferDBusTubeJob(const Nepomuk::PersonContact& contact,
                                   const QString serviceName,
                                   const QVariantMap& parameters,
                                   QObject* parent)
    : TelepathyBaseJob(*new OfferDBusTubeJobPrivate(contact,
                                                    serviceName,
                                                    parameters),
                       parent)
{
}


OfferDBusTubeJob::OfferDBusTubeJob(const Nepomuk::Person& metacontact,
                                   const QString serviceName,
                                   const QVariantMap& parameters,
                                   QObject* parent)
    : TelepathyBaseJob(*new OfferDBusTubeJobPrivate(metacontact,
                                                    serviceName,
                                                    parameters),
                       parent)
{
}


OfferDBusTubeJob::~OfferDBusTubeJob()
{
    kDebug();
}


void OfferDBusTubeJob::start()
{
    Q_D(OfferDBusTubeJob);

    // What are we supposed to do?
    switch (d->mode) {
        case OfferDBusTubeContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerDBusTubeContact()));
            break;
/* TODO?        case OfferDBusTubeContactServerMode:
            QTimer::singleShot(0, this, SLOT(__k__offerDBusTubeContactServer()));
            break;*/
        case OfferDBusTubeMetaContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerDBusTubeMetaContact()));
            break;
/* TODO?        case OfferDBusTubeMetaContactServerMode:
            QTimer::singleShot(0, this, SLOT(__k__offerDBusTubeMetaContactServer()));
            break;*/
        default:
            // Hmm?
            setError(TelepathyBridge::InvalidOperationError);
            setErrorText(i18n("This is an internal error of KTelepathy"));
            QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
            break;
    }
}


QDBusConnection OfferDBusTubeJob::dbusConnection()
{
    Q_D(OfferDBusTubeJob);
    return d->connection;
}


Tp::OutgoingDBusTubeChannelPtr OfferDBusTubeJob::outgoingDBusTubeChannel()
{
    Q_D(OfferDBusTubeJob);
    return d->channel;
}

OfferDBusTubeJobPrivate::OfferDBusTubeJobPrivate(const Nepomuk::PersonContact& c,
                                                 const QString s,
                                                 const QVariantMap& p)
    : mode(OfferDBusTubeJob::OfferDBusTubeContactMode),
      contact(c),
      serviceName(s),
      parameters(p),
      connection(QLatin1String("none"))
{
}


OfferDBusTubeJobPrivate::OfferDBusTubeJobPrivate(const Nepomuk::Person& m,
                                                 const QString s,
                                                 const QVariantMap& p)
    : mode(OfferDBusTubeJob::OfferDBusTubeMetaContactMode),
      metacontact(m),
      serviceName(s),
      parameters(p),
      connection(QLatin1String("none"))
{
}


OfferDBusTubeJobPrivate::~OfferDBusTubeJobPrivate()
{
}


void OfferDBusTubeJobPrivate::__k__offerDBusTubeContact()
{
    Q_Q(OfferDBusTubeJob);

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
        //TODO drdanz: supportsDBusTubes() not yet supported in TpQt4
        //if (i.key()->account()->connection()->capabilities()->supportsDBusTubes()) {
        if (1) {
            // Can target contact accept it?
            //TODO drdanz
            if (1) {
                uint contactHandle = i.value().first()->handle().at(0);

                QVariantMap request;
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"), QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE));
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"), (uint) Tp::HandleTypeContact);
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"), contactHandle);
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE ".ServiceName"), serviceName);
                kDebug() << i18n("Request:") << request;

                Tp::PendingChannel* pndChan = i.key()->account()->connection()->createChannel(request);
                q->connect(pndChan,
                           SIGNAL(finished(Tp::PendingOperation*)),
                           SLOT(__k__onDBusTubeChannelCreated(Tp::PendingOperation*)));
                return;
            }
        }
    }
    //TODO error dbustube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}

/* TODO?
void OfferDBusTubeJobPrivate::__k__offerDBusTubeContactServer()
{




}
*/

void OfferDBusTubeJobPrivate::__k__offerDBusTubeMetaContact()
{
    if(findBestContactFromMetaContact()) {
        __k__offerDBusTubeContact();
        return;
    }

    Q_Q(OfferDBusTubeJob);
    //TODO error dbustube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


/*TODO?
void OfferDBusTubeJobPrivate::__k__offerDBusTubeMetaContactServer()
{
    if(findBestContactFromMetaContact()) {
        __k__offerDBusTubeContactServer();
        return;
    }

    Q_Q(OfferDBusTubeJob);
    //TODO error dbustube not supported
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}
*/

void OfferDBusTubeJobPrivate::__k__onDBusTubeChannelCreated(Tp::PendingOperation* op)
{
    Q_Q(OfferDBusTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to create dbus tube channel") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }
    kDebug() << i18n("DBus tube channel created!");
    Q_EMIT q->infoMessage(q, i18n("DBus tube channel created!"));

    Tp::PendingChannel* pc = qobject_cast<Tp::PendingChannel*>(op);
    channel = Tp::OutgoingDBusTubeChannelPtr::dynamicCast(pc->channel());

    kDebug() << channel.data();

    q->connect(channel.data(),
            SIGNAL(invalidated(Tp::DBusProxy*, const QString&, const QString&)),
            SLOT(__k__onInvalidated()));
    Tp::Features features = Tp::Features() << Tp::TubeChannel::FeatureTube
                                           << Tp::DBusTubeChannel::FeatureDBusTube
                                           << Tp::DBusTubeChannel::FeatureBusNamesMonitoring;
    q->connect(channel->becomeReady(features),
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onDBusTubeChannelReady(Tp::PendingOperation*)));
}


void OfferDBusTubeJobPrivate::__k__onDBusTubeChannelReady(Tp::PendingOperation* op)
{
    Q_Q(OfferDBusTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to make dbus tube channel ready") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    kDebug() << i18n("DBus tube channel ready!");
    Q_EMIT q->infoMessage(q, i18n("DBus tube channel ready!"));

    q->connect(channel.data(),
               SIGNAL(busNamesChanged(QHash<Tp::ContactPtr,QString>,QList<Tp::ContactPtr>)),
               SLOT(onBusNamesChanged(QHash<Tp::ContactPtr,QString>,QList<Tp::ContactPtr>)));

    operations.clear();
    Tp::PendingOperation* offerTubeOperation = channel->offerTube(parameters);
    q->connect(offerTubeOperation, SIGNAL(finished(Tp::PendingOperation*)),
               q, SLOT(__k__onOfferTubeFinished(Tp::PendingOperation*)));
    addOperation(offerTubeOperation);
}


void OfferDBusTubeJobPrivate::__k__onOfferTubeFinished(Tp::PendingOperation* op)
{
    Q_Q(OfferDBusTubeJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to open dbus tube channel") <<"-" <<
            op->errorName() << ": " << op->errorMessage();
        return;
    }

    connection = channel->connection();

    kDebug() << i18n("DBus tube channel opened!");
    Q_EMIT q->infoMessage(q, i18n("DBus tube channel opened!"));
}


void OfferDBusTubeJobPrivate::__k__onBusNamesChanged(const QHash< Tp::ContactPtr, QString >& added,
                                                     const QList< Tp::ContactPtr >& removed )
{
    qDebug() << "Bus names changed" << added << removed;
}

void OfferDBusTubeJobPrivate::__k__onInvalidated()
{
    Q_Q(OfferDBusTubeJob);

    kWarning() << i18n("DBus tube invalidated!");
    Q_EMIT q->infoMessage(q, i18n("DBus tube invalidated!"));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


bool OfferDBusTubeJobPrivate::findBestContactFromMetaContact()
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

#include "offer-dbustube-job.moc"
