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

#include "requeststreamtubejob.h"
#include "abstractrequestchanneljob_p.h"

#include <TelepathyQt4/Account>
#include <KDebug>
#include "ontologies/imaccount.h"

class RequestStreamTubeJobPrivate : public AbstractRequestChannelJobPrivate
{
    Q_DECLARE_PUBLIC(RequestStreamTubeJob)

public:
    RequestStreamTubeJobPrivate(const Nepomuk::PersonContact& c,
                                const QString& s,
                                const QString& ph,
                                const KTelepathy::RequestChannelFlags rcf);
    RequestStreamTubeJobPrivate(const Nepomuk::Person& mc,
                                const QString& s,
                                const QString& ph,
                                const KTelepathy::RequestChannelFlags rcf);
    ~RequestStreamTubeJobPrivate();

    QString service;

    static inline RequestStreamTubeJob* newJob(const Nepomuk::PersonContact& contact,
                                               const QString& service,
                                               const QString& preferredHandler,
                                               const KTelepathy::RequestChannelFlags requestchannelflags,
                                               QObject* parent)
    {
        kDebug() << contact.genericLabel();
        kDebug() << service;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestStreamTubeJob *job = new RequestStreamTubeJob(*new RequestStreamTubeJobPrivate(contact,
                                                                                              service,
                                                                                              preferredHandler,
                                                                                              requestchannelflags),
                                                             parent);
        return job;
    }
    static inline RequestStreamTubeJob* newJob(const Nepomuk::Person& metacontact,
                                               const QString& service,
                                               const QString& preferredHandler,
                                               const KTelepathy::RequestChannelFlags requestchannelflags,
                                               QObject* parent)
    {
        kDebug() << metacontact.genericLabel();
        kDebug() << service;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestStreamTubeJob *job = new RequestStreamTubeJob(*new RequestStreamTubeJobPrivate(metacontact,
                                                                                              service,
                                                                                              preferredHandler,
                                                                                              requestchannelflags),
                                                             parent);
        return job;
    }
};


RequestStreamTubeJob::RequestStreamTubeJob(RequestStreamTubeJobPrivate &dd, QObject *parent)
    : AbstractRequestChannelJob(dd, parent)
{
}


RequestStreamTubeJob::~RequestStreamTubeJob()
{
}


Tp::PendingChannelRequest* RequestStreamTubeJob::createChannel()
{
    Q_D(RequestStreamTubeJob);
    kDebug() << d->contactResource.resourceUri() << d->contact->id();
    kDebug() << d->service;
    kDebug() << d->useractiontime;
    kDebug() << d->preferredHandler;
    return d->account->createStreamTube(d->contact, d->service, d->useractiontime, d->preferredHandler);
}


bool RequestStreamTubeJob::canCreateChannel()
{
    return true;
}


RequestStreamTubeJobPrivate::RequestStreamTubeJobPrivate(const Nepomuk::PersonContact& c,
                                                         const QString& s,
                                                         const QString& ph,
                                                         const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(c, ph, rcf),
      service(s)
{
    kDebug();
}


RequestStreamTubeJobPrivate::RequestStreamTubeJobPrivate(const Nepomuk::Person& mc,
                                                         const QString& s,
                                                         const QString& ph,
                                                         const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(mc, ph, rcf),
      service(s)
{
    kDebug();
}


RequestStreamTubeJobPrivate::~RequestStreamTubeJobPrivate()
{
    kDebug();
}

/*
RequestStreamTubeJob* requestStreamTube(const Nepomuk::PersonContact& contact,
                                        const QString& service,
                                        const QString& preferredHandler,
                                        const KTelepathy::RequestChannelFlags requestchannelflags,
                                        QObject* parent)
{
    return RequestStreamTubeJobPrivate::newJob(contact,
                                               service,
                                               preferredHandler,
                                               requestchannelflags,
                                               parent);

}

RequestStreamTubeJob* requestStreamTube(const Nepomuk::Person& metacontact,
                                        const QString preferredHandler,
                                        const KTelepathy::RequestChannelFlags requestchannelflags,
                                        QObject* parent)
{
    return RequestStreamTubeJobPrivate::newJob(metacontact,
                                               preferredHandler,
                                               requestchannelflags,
                                               parent);
}
*/

RequestStreamTubeJob* requestStreamTube(const Nepomuk::PersonContact& contact,
                                        const QString& service,
                                        const QString& preferredHandler,
                                        QObject* parent)
{
    kDebug();
    return RequestStreamTubeJobPrivate::newJob(contact,
                                               service,
                                               preferredHandler,
                                               KTelepathy::RequestChannelCreateMode,
                                               parent);

}

RequestStreamTubeJob* requestStreamTube(const Nepomuk::Person& metacontact,
                                        const QString& service,
                                        const QString& preferredHandler,
                                        QObject* parent)
{
    kDebug();
    return RequestStreamTubeJobPrivate::newJob(metacontact,
                                               service,
                                               preferredHandler,
                                               KTelepathy::RequestChannelCreateMode,
                                               parent);
}

#include "requeststreamtubejob.moc"
