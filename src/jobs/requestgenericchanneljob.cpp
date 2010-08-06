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

#include "requestgenericchanneljob.h"
#include "abstractrequestchanneljob_p.h"

#include <TelepathyQt4/ReferencedHandles>
#include <TelepathyQt4/Account>
#include <KDebug>


class RequestGenericChannelJobPrivate : public AbstractRequestChannelJobPrivate
{
    Q_DECLARE_PUBLIC(RequestGenericChannelJob)

public:
    RequestGenericChannelJobPrivate(const Nepomuk::PersonContact& c,
                                    const QVariantMap& rq,
                                    const QString& ph,
                                    const KTelepathy::RequestChannelFlags rcf);
    RequestGenericChannelJobPrivate(const Nepomuk::Person& mc,
                                    const QVariantMap& rq,
                                    const QString& ph,
                                    const KTelepathy::RequestChannelFlags rcf);
    RequestGenericChannelJobPrivate(const QString& r,
                                    const QVariantMap& rq,
                                    const QString& ph,
                                    const KTelepathy::RequestChannelFlags rcf);
    ~RequestGenericChannelJobPrivate();

    QVariantMap request;

    static inline RequestGenericChannelJob* newJob( const Nepomuk::PersonContact& contact,
                                                    const QVariantMap& request,
                                                    const QString& preferredHandler,
                                                    const KTelepathy::RequestChannelFlags requestchannelflags,
                                                    QObject* parent)
    {
        kDebug() << contact.genericLabel();
        kDebug() << request;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestGenericChannelJob *job = new RequestGenericChannelJob(*new RequestGenericChannelJobPrivate(contact,
                                                                                                          request,
                                                                                                          preferredHandler,
                                                                                                          requestchannelflags),
                                                                     parent);
        return job;
    }
    static inline RequestGenericChannelJob* newJob( const Nepomuk::Person& metacontact,
                                                    const QVariantMap& request,
                                                    const QString& preferredHandler,
                                                    const KTelepathy::RequestChannelFlags requestchannelflags,
                                                    QObject* parent)
    {
        kDebug() << metacontact.genericLabel();
        kDebug() << request;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestGenericChannelJob *job = new RequestGenericChannelJob(*new RequestGenericChannelJobPrivate(metacontact,
                                                                                                          request,
                                                                                                          preferredHandler,
                                                                                                          requestchannelflags),
                                                                     parent);
        return job;
    }
    static inline RequestGenericChannelJob* newJob( const QString room,
                                                    const QVariantMap& request,
                                                    const QString& preferredHandler,
                                                    const KTelepathy::RequestChannelFlags requestchannelflags,
                                                    QObject* parent)
    {
        kDebug() << room;
        kDebug() << request;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestGenericChannelJob *job = new RequestGenericChannelJob(*new RequestGenericChannelJobPrivate(room,
                                                                                                          request,
                                                                                                          preferredHandler,
                                                                                                          requestchannelflags),
                                                                     parent);
        return job;
    }
};


RequestGenericChannelJob::RequestGenericChannelJob(RequestGenericChannelJobPrivate &dd, QObject *parent)
    : AbstractRequestChannelJob(dd, parent)
{
}


RequestGenericChannelJob::~RequestGenericChannelJob()
{
}


Tp::PendingChannelRequest* RequestGenericChannelJob::ensureChannel()
{
    kDebug();
    Q_D(RequestGenericChannelJob);
    updateRequestTargets();
    return d->account->ensureChannel(d->request, d->useractiontime, d->preferredHandler);
}


Tp::PendingChannelRequest* RequestGenericChannelJob::createChannel()
{
    kDebug();
    Q_D(RequestGenericChannelJob);
    updateRequestTargets();
    return d->account->createChannel(d->request, d->useractiontime, d->preferredHandler);
}


bool RequestGenericChannelJob::canEnsureChannel()
{
    return true;
}


bool RequestGenericChannelJob::canCreateChannel()
{
    return true;
}


void RequestGenericChannelJob::updateRequestTargets()
{
    Q_D(RequestGenericChannelJob);
    kDebug();
    switch (d->targetmode) {
        case AbstractRequestChannelJob::TargetModeContact:
        case AbstractRequestChannelJob::TargetModeMetaContact:

            if (d->request.contains(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"))) {
                kWarning() << QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType") << i18n("already set. Removing all values");
                d->request.remove(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"));
            }
            d->request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"),
                        (uint) Tp::HandleTypeContact);

            if (d->request.contains(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"))) {
                kWarning() << QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle") << i18n("already set. Removing all values");
                d->request.remove(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"));
            }
            d->request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"),
                        d->contact->handle().at(0));
            break;

        case AbstractRequestChannelJob::TargetModeRoom:
        case AbstractRequestChannelJob::TargetModeError:
        default:
            kWarning() << __LINE__ << "TODO";
    }

    kDebug() << i18n("Request:") << d->request;
}


RequestGenericChannelJobPrivate::RequestGenericChannelJobPrivate(const Nepomuk::PersonContact& c,
                                                                 const QVariantMap& rq,
                                                                 const QString& ph,
                                                                 const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(c, ph, rcf),
      request(rq)
{
    kDebug();
}


RequestGenericChannelJobPrivate::RequestGenericChannelJobPrivate(const Nepomuk::Person& mc,
                                                                 const QVariantMap& rq,
                                                                 const QString& ph,
                                                                 const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(mc, ph, rcf),
      request(rq)
{
    kDebug();
}


RequestGenericChannelJobPrivate::RequestGenericChannelJobPrivate(const QString& r,
                                                                 const QVariantMap& rq,
                                                                 const QString& ph,
                                                                 const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(r, ph, rcf),
      request(rq)
{
    kDebug();
}


RequestGenericChannelJobPrivate::~RequestGenericChannelJobPrivate()
{
    kDebug();
}
