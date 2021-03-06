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

#include "abstractrequestchanneljob.h"
#include "abstractrequestchanneljob_p.h"

#include "telepathy-bridge.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"
#include "ontologies/imaccount.h"

#include <TelepathyQt4/PendingChannelRequest>
#include <TelepathyQt4/Account>

#include <KDebug>
#include <KLocalizedString>


using namespace KTelepathy;

AbstractRequestChannelJob::AbstractRequestChannelJob(AbstractRequestChannelJobPrivate &dd, QObject *parent)
    : TelepathyBaseJob(dd, parent)
{
    setCapabilities(KJob::Killable);
    connect(this, SIGNAL(requestChannelFinished()), SLOT(onRequestChannelFinished()));
}


AbstractRequestChannelJob::~AbstractRequestChannelJob() {}


void AbstractRequestChannelJob::start()
{
    QTimer::singleShot(0, this, SLOT(__k__requestChannel()));
}


bool AbstractRequestChannelJob::kill(KJob::KillVerbosity verbosity) //TODO
{
    Q_UNUSED(verbosity)
    Q_D(AbstractRequestChannelJob);
    if(d->pendingchannelrequest)
        d->pendingchannelrequest->cancel();
    return true;
}


Tp::PendingChannelRequest* AbstractRequestChannelJob::ensureChannel()
{
    kWarning() << "This method should not be called.";
    setError(KJob::UserDefinedError);
    setErrorText(i18n("This is an internal error of KTelepathy"));
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    return NULL;
}


Tp::PendingChannelRequest* AbstractRequestChannelJob::createChannel()
{
    kWarning() << "This method should not be called.";
    setError(KJob::UserDefinedError);
    setErrorText(i18n("This is an internal error of KTelepathy"));
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    return NULL;
}


bool AbstractRequestChannelJob::canEnsureChannel()
{ 
    return false;
}


bool AbstractRequestChannelJob::canCreateChannel()
{
    return false;
}


void AbstractRequestChannelJob::onRequestChannelFinished()
{
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
}


AbstractRequestChannelJobPrivate::AbstractRequestChannelJobPrivate( const Nepomuk::PersonContact& c,
                                                                    const QString& ph,
                                                                    const KTelepathy::RequestChannelFlags rcf)
    : TelepathyBaseJobPrivate(),
      targetmode(AbstractRequestChannelJob::TargetModeContact),
      requestchannelflags(rcf),
      contactResource(c),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      pendingchannelrequest(0)
{
    kDebug();
    initTargets();
}

AbstractRequestChannelJobPrivate::AbstractRequestChannelJobPrivate( const Nepomuk::Person& mc,
                                                                    const QString& ph,
                                                                    const KTelepathy::RequestChannelFlags rcf)
    : TelepathyBaseJobPrivate(),
      targetmode(AbstractRequestChannelJob::TargetModeMetaContact),
      requestchannelflags(rcf),
      metacontactResource(mc),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      pendingchannelrequest(0)
{
    kDebug();
    initTargets();
}


AbstractRequestChannelJobPrivate::AbstractRequestChannelJobPrivate( const QString& r,
                                                                    const QString& ph,
                                                                    const KTelepathy::RequestChannelFlags rcf)
    : TelepathyBaseJobPrivate(),
      targetmode(AbstractRequestChannelJob::TargetModeRoom),
      requestchannelflags(rcf),
      room(r),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      pendingchannelrequest(0)
{
    kDebug();
    initTargets();
}

/*
AbstractRequestChannelJobPrivate::AbstractRequestChannelJobPrivate( const Nepomuk::IMAccount& a,
                                                                    const Nepomuk::PersonContact& c,
                                                                    const QString& ph,
                                                                    const KTelepathy::RequestChannelFlags rcf)
    : TelepathyBaseJobPrivate(),
      targetmode(AbstractRequestChannelJob::TargetModeContact),
      requestchannelflags(rcf),
      accountResource(a),
      contactResource(c),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      pendingchannelrequest(0)
{
    kDebug();
    initTargets();
}
*/
/*
AbstractRequestChannelJobPrivate::AbstractRequestChannelJobPrivate( const Nepomuk::IMAccount& a,
                                                                    const Nepomuk::Person& mc,
                                                                    const QString& ph,
                                                                    const KTelepathy::RequestChannelFlags rcf)
    : TelepathyBaseJobPrivate(),
      targetmode(AbstractRequestChannelJob::TargetModeMetaContact),
      requestchannelflags(rcf),
      accountResource(a),
      metacontactResource(mc),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      pendingchannelrequest(0)
{
    kDebug();
    initTargets();
}
*/
AbstractRequestChannelJobPrivate::AbstractRequestChannelJobPrivate( const Nepomuk::IMAccount& a,
                                                                    const QString& r,
                                                                    const QString& ph,
                                                                    const KTelepathy::RequestChannelFlags rcf)
    : TelepathyBaseJobPrivate(),
      targetmode(AbstractRequestChannelJob::TargetModeAccountRoom),
      requestchannelflags(rcf),
      accountResource(a),
      room(r),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      pendingchannelrequest(0)
{
    kDebug();
    initTargets();
}

AbstractRequestChannelJobPrivate::~AbstractRequestChannelJobPrivate()
{
    kDebug();
}

void AbstractRequestChannelJobPrivate::__k__requestChannel()
{
    Q_Q(AbstractRequestChannelJob);

    //Handle some possible errors
    if(pendingchannelrequest) {
        kWarning() << "What happened?";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (!q->canEnsureChannel() && !q->canCreateChannel()) {
        kWarning() << "This job is useless!";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (!(requestchannelflags & (KTelepathy::RequestChannelEnsureMode | KTelepathy::RequestChannelCreateMode))) {
        kWarning() << "What do you whant to request?";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }


    // Request the channel
    if (requestchannelflags & KTelepathy::RequestChannelEnsureMode) {
        if (q->canEnsureChannel()) {
            pendingchannelrequest = q->ensureChannel();
        } else if (q->canCreateChannel()) {
            kWarning() << "RequestChannelEnsureMode not supported, using RequestChannelCreateMode";
            pendingchannelrequest = q->createChannel();
        }
    } else if (requestchannelflags & KTelepathy::RequestChannelCreateMode) {
        if (q->canCreateChannel()) {
            pendingchannelrequest = q->createChannel();
        } else if (q->canEnsureChannel()) {
            kWarning() << "RequestChannelCreateMode not supported, using RequestChannelEnsureMode";
            pendingchannelrequest = q->createChannel();
        }
    } else {
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }


    // Check if the channel request was created...
    if(!pendingchannelrequest) {
        kWarning() << "Pending channel request was not created.";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    q->connect(pendingchannelrequest,
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onRequestChannelFinished(Tp::PendingOperation*)));
}


void AbstractRequestChannelJobPrivate::__k__onRequestChannelFinished(Tp::PendingOperation* op)
{
    Q_Q(AbstractRequestChannelJob);
    if (op->isError()) {
        kWarning() << "Channel request failed:"
                   << op->errorName()
                   << op->errorMessage();
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("An error occurred requesting the channel."));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
    }

    Q_EMIT q->requestChannelFinished();
}


void AbstractRequestChannelJobPrivate::initTargets()
{
    Q_Q(AbstractRequestChannelJob);

    switch (targetmode) {
        case AbstractRequestChannelJob::TargetModeContact:
            initTargetsModeContact();
            break;
        case AbstractRequestChannelJob::TargetModeMetaContact:
            initTargetsModeMetaContact();
            break;
        case AbstractRequestChannelJob::TargetModeRoom:
            initTargetsModeRoom();
            break;
        case AbstractRequestChannelJob::TargetModeAccountRoom:
            initTargetsModeAccountRoom();
            break;
        case AbstractRequestChannelJob::TargetModeRoomContactsList:
        case AbstractRequestChannelJob::TargetModeRoomMetaContactsList:
        case AbstractRequestChannelJob::TargetModeAccountContact:
        case AbstractRequestChannelJob::TargetModeAccountMetaContact:
        case AbstractRequestChannelJob::TargetModeAccountRoomContactsList:
        case AbstractRequestChannelJob::TargetModeAccountRoomMetaContactsList:
        case AbstractRequestChannelJob::TargetModeError:
        default:
            // Hmm?
            q->setError(KJob::UserDefinedError);
            q->setErrorText(i18n("This is an internal error of KTelepathy"));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            break;
    }
}


void AbstractRequestChannelJobPrivate::initTargetsModeContact()
{
    kDebug();
    Q_Q(AbstractRequestChannelJob);

    QHash< TelepathyAccountProxy*, QList< Tp::ContactPtr > > tbd;
    QHash< TelepathyAccountProxy*, QList< Nepomuk::PersonContact > > proxyToContacts;

    QStringList imIDs;

    foreach (const Nepomuk::IMAccount& account, contactResource.iMAccounts()) {
        imIDs << account.imIDs();
    }

    foreach (TelepathyAccountProxy* proxy, TelepathyBridge::instance()->d_func()->accountProxiesForContact(contactResource)) {
        tbd[proxy] << proxy->contactsForIdentifiers(imIDs);
        proxyToContacts[proxy] << contactResource;
    }

    if (tbd.isEmpty()) {
        kWarning() << "No contacts retrieved!";
        // Give it up and emit an error
        q->setError(TelepathyBridge::NoContactsFoundError);
        q->setErrorText(i18n("No existing Telepathy contacts could be mapped to the chosen contact"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

//TODO check if target contact is online?

    QHash< TelepathyAccountProxy*, QList< Tp::ContactPtr > >::const_iterator i;
    for (i = tbd.constBegin(); i != tbd.constEnd(); ++i) {
        // TODO Can we do it?
        if (1) {
            // TODO Can target contact accept it?
            if (1) {
                account = i.key()->account();
                if(!account)
                    kWarning() << "!account";
                contact = i.value().first();
                if(!account)
                    kWarning() << "!contact";
                return;
            }
        }
    }
    q->setError(TelepathyBridge::NoContactsFoundError);
    q->setErrorText(i18n("No existing Telepathy contacts could be mapped to the chosen contact"));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void AbstractRequestChannelJobPrivate::initTargetsModeMetaContact()
{
    kDebug();
    Q_Q(AbstractRequestChannelJob);

    foreach(Nepomuk::PersonContact foundContact, TelepathyBridge::instance()->d_func()->contactsForMetaContact(metacontactResource)) {
        // TODO Can we do it?
        if (1) {
            // TODO Can target contact accept it?
            if (1) {
                contactResource = foundContact;
                initTargetsModeContact();
                return;
            }
        }
    }
    q->setError(TelepathyBridge::NoContactsFoundError);
    q->setErrorText(i18n("No existing Telepathy contacts could be mapped to the chosen metacontact"));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void AbstractRequestChannelJobPrivate::initTargetsModeRoom()
{
    kDebug();
    QList< TelepathyAccountProxy* > proxies = TelepathyBridge::instance()->d_func()->accountProxies;
    // TODO order by preference?

    foreach(TelepathyAccountProxy* proxy, proxies)
    {
        kWarning() << "Using the first account!"; //TODO check for capabilities
        account = proxy->account();
        if (account->isValid())
            return;
    }

    Q_Q(AbstractRequestChannelJob);
    q->setError(KJob::UserDefinedError);
    q->setErrorText(i18n("Cannot find a valid account."));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void AbstractRequestChannelJobPrivate::initTargetsModeAccountRoom()
{
    kDebug();
    TelepathyAccountProxy* proxy = TelepathyBridge::instance()->d_func()->accountProxyForAccount(accountResource);
    account = proxy->account();
    if (!account->isValid())
    {
        Q_Q(AbstractRequestChannelJob);
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("Requested account is not valid."));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
    }
}

#include "abstractrequestchanneljob.moc"
