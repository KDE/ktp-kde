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

#include "requestchanneljob.h"
#include "requestchanneljob_p.h"
#include "telepathy-bridge.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"
#include "ontologies/imaccount.h"

#include <TelepathyQt4/PendingChannelRequest>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/ReferencedHandles>

#include <KMessageBox>
#include <KDebug>


RequestChannelJob::RequestChannelJob(RequestChannelJobPrivate &dd, QObject *parent)
    : TelepathyBaseJob(dd, parent)
{
    kDebug();
    setCapabilities(KJob::Killable);
}

RequestChannelJob::~RequestChannelJob()
{
    kDebug();
}


void RequestChannelJob::start()
{
    kDebug();
    QTimer::singleShot(0, this, SLOT(__k__requestChannel()));
}


bool RequestChannelJob::kill(KJob::KillVerbosity verbosity) //TODO
{
    kDebug();
    Q_UNUSED(verbosity)
    Q_D(RequestChannelJob);
    if(d->m_pendingChannelRequest)
        d->m_pendingChannelRequest->cancel();
    return true;
}


Tp::PendingChannelRequest* RequestChannelJob::ensureChannel()
{
    kDebug();
    Q_D(RequestChannelJob);
    d->updateRequestTargets();
    return d->account->ensureChannel(d->request, d->useractiontime, d->preferredHandler);
}


Tp::PendingChannelRequest* RequestChannelJob::createChannel()
{
    kDebug();
    Q_D(RequestChannelJob);
    d->updateRequestTargets();
    return d->account->createChannel(d->request, d->useractiontime, d->preferredHandler);
}

bool inline RequestChannelJob::canEnsureChannel() { return true; }
bool inline RequestChannelJob::canCreateChannel() { return true; }


RequestChannelJobPrivate::RequestChannelJobPrivate( const Nepomuk::PersonContact& c,
                                                    const QVariantMap& rq,
                                                    const QString ph,
                                                    const RequestChannelJob::RequestFlags f)
    : targetmode(RequestChannelJob::TargetModeContact),
      requestflags(f),
      contactResource(c),
      request(rq),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      m_pendingChannelRequest(0)
{
    kDebug();
    initTargets();
}

RequestChannelJobPrivate::RequestChannelJobPrivate( const Nepomuk::Person& mc,
                                                    const QVariantMap& rq,
                                                    const QString ph,
                                                    const RequestChannelJob::RequestFlags f)
    : targetmode(RequestChannelJob::TargetModeMetaContact),
      requestflags(f),
      metacontactResource(mc),
      request(rq),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      m_pendingChannelRequest(0)
{
    kDebug();
    initTargets();
}

RequestChannelJobPrivate::RequestChannelJobPrivate( const QString r,
                                                    const QVariantMap& rq,
                                                    const QString ph,
                                                    const RequestChannelJob::RequestFlags f)
    : targetmode(RequestChannelJob::TargetModeRoom),
      requestflags(f),
      room(r),
      request(rq),
      useractiontime(QDateTime::currentDateTime()),
      preferredHandler(ph),
      m_pendingChannelRequest(0)
{
    kDebug();
    initTargets();
}


RequestChannelJobPrivate::~RequestChannelJobPrivate()
{
    kDebug();
    initTargets();
}


void RequestChannelJobPrivate::__k__requestChannel()
{
    kDebug();
    Q_Q(RequestChannelJob);

    //Handle some possible errors
    if(m_pendingChannelRequest) {
        kWarning() << "What happened?";
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (!(requestflags & (RequestChannelJob::RequestModeEnsure | RequestChannelJob::RequestModeCreate))) {
        kWarning() << "What do you whant to request?";
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (!q->canEnsureChannel() && !q->canCreateChannel()) {
        kWarning() << "This job is useless!";
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (requestflags & RequestChannelJob::RequestModeEnsure) {
        if (q->canEnsureChannel()) {
            m_pendingChannelRequest = q->ensureChannel();
        } else if (q->canCreateChannel()) {
            kWarning() << "RequestModeEnsure not supported, using RequestModeCreate";
            m_pendingChannelRequest = q->createChannel();
        }
    } else if (requestflags & RequestChannelJob::RequestModeCreate) {
        if (q->canCreateChannel()) {
            m_pendingChannelRequest = q->createChannel();
        } else if (q->canEnsureChannel()) {
            kWarning() << "RequestModeCreate not supported, using RequestModeEnsure";
            m_pendingChannelRequest = q->createChannel();
        }
    } else {
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    // Check if the channel request was created...
    if(!m_pendingChannelRequest) {
        kWarning() << "!m_pendingChannelRequest";
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    q->connect(m_pendingChannelRequest,
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onPendingChannelRequestFinished(Tp::PendingOperation*)));
}


void RequestChannelJobPrivate::initTargets()
{
    kDebug();
    Q_Q(RequestChannelJob);

    switch (targetmode) {
        case RequestChannelJob::TargetModeContact:
            initTargetsModeContact();
            break;
        case RequestChannelJob::TargetModeMetaContact:
            initTargetsModeMetaContact();
            break;
        case RequestChannelJob::TargetModeRoom:
            initTargetsModeMetaContact();
            break;
        case RequestChannelJob::TargetModeError:
        default:
            // Hmm?
            q->setError(TelepathyBridge::InvalidOperationError);
            q->setErrorText(i18n("This is an internal error of KTelepathy"));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            break;
    }
}

void RequestChannelJobPrivate::initTargetsModeContact()
{
    kDebug();
    Q_Q(RequestChannelJob);

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

void RequestChannelJobPrivate::initTargetsModeMetaContact()
{
    kDebug();
    Q_Q(RequestChannelJob);

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


void RequestChannelJobPrivate::updateRequestTargets()
{
    kDebug();
    switch (targetmode) {
        case RequestChannelJob::TargetModeContact:
        case RequestChannelJob::TargetModeMetaContact:

            if (request.contains(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"))) {
                kWarning() << QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType") << i18n("already set. Removing all values");
                request.remove(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"));
            }
            request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"),
                        (uint) Tp::HandleTypeContact);

            if (request.contains(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"))) {
                kWarning() << QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle") << i18n("already set. Removing all values");
                request.remove(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"));
            }
            request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"),
                        contact->handle()[0]);
            break;

        case RequestChannelJob::TargetModeRoom:
        case RequestChannelJob::TargetModeError:
        default:
            kWarning() << __LINE__ << "TODO";
    }

    kDebug() << i18n("Request:") << request;
}


void RequestChannelJobPrivate::__k__onPendingChannelRequestFinished(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(RequestChannelJob);
    kDebug();
    if (op->isError()) {
        kWarning() << "Channel request failed:"
                   << op->errorName()
                   << op->errorMessage();
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("An error occourred requesting channel."));
    }

    Q_EMIT q->infoMessage(q, i18n("Channel request finished."));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}



RequestChannelJob* requestChannel(const Nepomuk::PersonContact& contact,
                                  const QVariantMap& request,
                                  const QString preferredHandler,
                                  const RequestChannelJob::RequestFlag requestflags,
                                  QObject* parent)
{
    kDebug();
    return RequestChannelJobPrivate::newJob(contact,
                                            request,
                                            preferredHandler,
                                            requestflags,
                                            parent);
}

RequestChannelJob* requestChannel(const Nepomuk::Person& metacontact,
                                  const QVariantMap& request,
                                  const QString preferredHandler,
                                  const RequestChannelJob::RequestFlag requestflags,
                                  QObject* parent)
{
    kDebug();
    return RequestChannelJobPrivate::newJob(metacontact,
                                            request,
                                            preferredHandler,
                                            requestflags,
                                            parent);
}

RequestChannelJob* requestChannel(const QString room,
                                  const QVariantMap& request,
                                  const QString preferredHandler,
                                  const RequestChannelJob::RequestFlag requestflags,
                                  QObject* parent)
{
    kDebug();
    return RequestChannelJobPrivate::newJob(room,
                                            request,
                                            preferredHandler,
                                            requestflags,
                                            parent);
}

#include "requestchanneljob.moc"
