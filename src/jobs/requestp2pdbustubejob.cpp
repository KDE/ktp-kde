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

#include "requestp2pdbustubejob.h"
#include "abstractrequestchanneljob_p.h"

#include <TelepathyQt4/Account>
#include <KDebug>
#include "ontologies/imaccount.h"


namespace KTelepathy {

class RequestP2PDBusTubeJobPrivate : public AbstractRequestChannelJobPrivate
{
    Q_DECLARE_PUBLIC(RequestP2PDBusTubeJob)

public:
    RequestP2PDBusTubeJobPrivate(const Nepomuk::PersonContact& c,
                                 const QString& sn,
                                 const QString& ph,
                                 const KTelepathy::RequestChannelFlags rcf);
    RequestP2PDBusTubeJobPrivate(const Nepomuk::Person& mc,
                                 const QString& sn,
                                 const QString& ph,
                                 const KTelepathy::RequestChannelFlags rcf);
    ~RequestP2PDBusTubeJobPrivate();

    QString serviceName;

    static inline RequestP2PDBusTubeJob* newJob(const Nepomuk::PersonContact& contact,
                                                const QString& serviceName,
                                                const QString& preferredHandler,
                                                const KTelepathy::RequestChannelFlags requestchannelflags,
                                                QObject* parent)
    {
        kDebug() << contact.genericLabel();
        kDebug() << serviceName;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestP2PDBusTubeJob *job = new RequestP2PDBusTubeJob(*new RequestP2PDBusTubeJobPrivate(contact,
                                                                                                 serviceName,
                                                                                                 preferredHandler,
                                                                                                 requestchannelflags),
                                                               parent);
        return job;
    }
    static inline RequestP2PDBusTubeJob* newJob(const Nepomuk::Person& metacontact,
                                                const QString& serviceName,
                                                const QString& preferredHandler,
                                                const KTelepathy::RequestChannelFlags requestchannelflags,
                                                QObject* parent)
    {
        kDebug() << metacontact.genericLabel();
        kDebug() << serviceName;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestP2PDBusTubeJob *job = new RequestP2PDBusTubeJob(*new RequestP2PDBusTubeJobPrivate(metacontact,
                                                                                                 serviceName,
                                                                                                 preferredHandler,
                                                                                                 requestchannelflags),
                                                               parent);
        return job;
    }
};

} // namespace KTelepathy

using namespace KTelepathy;

RequestP2PDBusTubeJob::RequestP2PDBusTubeJob(RequestP2PDBusTubeJobPrivate &dd, QObject *parent)
    : AbstractRequestChannelJob(dd, parent)
{
}


RequestP2PDBusTubeJob::~RequestP2PDBusTubeJob()
{
}


Tp::PendingChannelRequest* RequestP2PDBusTubeJob::createChannel()
{
    Q_D(RequestP2PDBusTubeJob);
    kDebug() << d->contactResource.resourceUri() << d->contact->id();
    kDebug() << d->serviceName;
    kDebug() << d->useractiontime;
    kDebug() << d->preferredHandler;
    return d->account->createP2PDBusTube(d->contact, d->serviceName, d->useractiontime, d->preferredHandler);
}


bool RequestP2PDBusTubeJob::canCreateChannel()
{
    return true;
}


RequestP2PDBusTubeJobPrivate::RequestP2PDBusTubeJobPrivate(const Nepomuk::PersonContact& c,
                                                           const QString& sn,
                                                           const QString& ph,
                                                           const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(c, ph, rcf),
      serviceName(sn)
{
    kDebug();
}


RequestP2PDBusTubeJobPrivate::RequestP2PDBusTubeJobPrivate(const Nepomuk::Person& mc,
                                                           const QString& sn,
                                                           const QString& ph,
                                                           const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(mc, ph, rcf),
      serviceName(sn)
{
    kDebug();
}


RequestP2PDBusTubeJobPrivate::~RequestP2PDBusTubeJobPrivate()
{
    kDebug();
}


namespace KTelepathy {

/*
RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::PersonContact& contact,
                                          const QString& serviceName,
                                          const QString& preferredHandler,
                                          const KTelepathy::RequestChannelFlags requestchannelflags,
                                          QObject* parent)
{
    return RequestP2PDBusTubeJobPrivate::newJob(contact,
                                                serviceName,
                                                preferredHandler,
                                                requestchannelflags,
                                                parent);

}

RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::Person& metacontact,
                                    const QString preferredHandler,
                                    const KTelepathy::RequestChannelFlags requestchannelflags,
                                    QObject* parent)
{
    return RequestP2PDBusTubeJobPrivate::newJob(metacontact,
                                             preferredHandler,
                                             requestchannelflags,
                                             parent);
}
*/

RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::PersonContact& contact,
                                          const QString& serviceName,
                                          const QString& preferredHandler,
                                          QObject* parent)
{
    kDebug();
    return RequestP2PDBusTubeJobPrivate::newJob(contact,
                                                serviceName,
                                                preferredHandler,
                                                KTelepathy::RequestChannelCreateMode,
                                                parent);

}

RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::Person& metacontact,
                                          const QString& serviceName,
                                          const QString& preferredHandler,
                                          QObject* parent)
{
    kDebug();
    return RequestP2PDBusTubeJobPrivate::newJob(metacontact,
                                                serviceName,
                                                preferredHandler,
                                                KTelepathy::RequestChannelCreateMode,
                                                parent);
}

} // namespace KTelepathy

#include "requestp2pdbustubejob.moc"
