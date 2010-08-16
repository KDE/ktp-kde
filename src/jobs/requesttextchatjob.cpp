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

#include "requesttextchatjob.h"
#include "abstractrequestchanneljob_p.h"

#include <TelepathyQt4/Account>
#include <KDebug>
#include "ontologies/imaccount.h"

namespace KTelepathy {

class RequestTextChatJobPrivate : public AbstractRequestChannelJobPrivate
{
    Q_DECLARE_PUBLIC(RequestTextChatJob)

public:
    RequestTextChatJobPrivate(const Nepomuk::PersonContact& c,
                              const QString& ph,
                              const KTelepathy::RequestChannelFlags rcf);
    RequestTextChatJobPrivate(const Nepomuk::Person& mc,
                              const QString& ph,
                              const KTelepathy::RequestChannelFlags rcf);
    ~RequestTextChatJobPrivate();

    static inline RequestTextChatJob* newJob(const Nepomuk::PersonContact& contact,
                                             const QString& preferredHandler,
                                             const KTelepathy::RequestChannelFlags requestchannelflags,
                                             QObject* parent)
    {
        kDebug() << contact.genericLabel();
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestTextChatJob *job = new RequestTextChatJob(*new RequestTextChatJobPrivate(contact,
                                                                                        preferredHandler,
                                                                                        requestchannelflags),
                                                         parent);
        return job;
    }
    static inline RequestTextChatJob* newJob(const Nepomuk::Person& metacontact,
                                             const QString& preferredHandler,
                                             const KTelepathy::RequestChannelFlags requestchannelflags,
                                             QObject* parent)
    {
        kDebug() << metacontact.genericLabel();
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestTextChatJob *job = new RequestTextChatJob(*new RequestTextChatJobPrivate(metacontact,
                                                                                        preferredHandler,
                                                                                        requestchannelflags),
                                                         parent);
        return job;
    }
};

} // namespace KTelepathy

using namespace KTelepathy;

RequestTextChatJob::RequestTextChatJob(RequestTextChatJobPrivate &dd, QObject *parent)
    : AbstractRequestChannelJob(dd, parent)
{
}


RequestTextChatJob::~RequestTextChatJob()
{
}


Tp::PendingChannelRequest* RequestTextChatJob::ensureChannel()
{
    Q_D(RequestTextChatJob);
    kDebug() << d->contactResource.resourceUri() << d->contact->id();
    kDebug() << d->useractiontime;
    kDebug() << d->preferredHandler;
    return d->account->ensureTextChat(d->contact, d->useractiontime, d->preferredHandler);
}


bool RequestTextChatJob::canEnsureChannel()
{
    return true;
}


RequestTextChatJobPrivate::RequestTextChatJobPrivate(const Nepomuk::PersonContact& c,
                                                     const QString& ph,
                                                     const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(c, ph, rcf)
{
    kDebug();
}


RequestTextChatJobPrivate::RequestTextChatJobPrivate(const Nepomuk::Person& mc,
                                                     const QString& ph,
                                                     const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(mc, ph, rcf)
{
    kDebug();
}


RequestTextChatJobPrivate::~RequestTextChatJobPrivate()
{
    kDebug();
}

/*
RequestTextChatJob* requestTextChat(const Nepomuk::PersonContact& contact,
                                    const QString& preferredHandler,
                                    const KTelepathy::RequestChannelFlags requestchannelflags,
                                    QObject* parent)
{
    return RequestTextChatJobPrivate::newJob(contact,
                                             preferredHandler,
                                             requestchannelflags,
                                             parent);

}

RequestTextChatJob* requestTextChat(const Nepomuk::Person& metacontact,
                                    const QString& preferredHandler,
                                    const KTelepathy::RequestChannelFlags requestchannelflags,
                                    QObject* parent)
{
    return RequestTextChatJobPrivate::newJob(metacontact,
                                             preferredHandler,
                                             requestchannelflags,
                                             parent);
}
*/

RequestTextChatJob* requestTextChat(const Nepomuk::PersonContact& contact,
                                    const QString& preferredHandler,
                                    QObject* parent)
{
    kDebug();
    return RequestTextChatJobPrivate::newJob(contact,
                                             preferredHandler,
                                             KTelepathy::RequestChannelEnsureMode,
                                             parent);

}

RequestTextChatJob* requestTextChat(const Nepomuk::Person& metacontact,
                                    const QString& preferredHandler,
                                    QObject* parent)
{
    kDebug();
    return RequestTextChatJobPrivate::newJob(metacontact,
                                             preferredHandler,
                                             KTelepathy::RequestChannelEnsureMode,
                                             parent);
}

#include "requesttextchatjob.moc"
