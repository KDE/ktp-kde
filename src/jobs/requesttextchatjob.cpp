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
#include "requestchanneljob_p.h"

#include <TelepathyQt4/Account>
#include <KDebug>
#include "ontologies/imaccount.h"

class RequestTextChatJobPrivate : public RequestChannelJobPrivate
{
    Q_DECLARE_PUBLIC(RequestTextChatJob)

public:
    RequestTextChatJobPrivate(const Nepomuk::PersonContact& c,
                              const QString ph);
    RequestTextChatJobPrivate(const Nepomuk::Person& mc,
                              const QString ph);
    ~RequestTextChatJobPrivate();

    static inline RequestTextChatJob* newJob(const Nepomuk::PersonContact& contact,
                                            const QString preferredHandler,
                                            QObject* parent)
    {
        kDebug();
        RequestTextChatJob *job = new RequestTextChatJob(*new RequestTextChatJobPrivate(contact, preferredHandler), parent);
        return job;
    }
    static inline RequestTextChatJob* newJob(const Nepomuk::Person& metacontact,
                                            const QString preferredHandler,
                                            QObject* parent)
    {
        kDebug();
        RequestTextChatJob *job = new RequestTextChatJob(*new RequestTextChatJobPrivate(metacontact, preferredHandler), parent);
        return job;
    }
};

RequestTextChatJob::RequestTextChatJob(RequestTextChatJobPrivate &dd, QObject *parent)
    : RequestChannelJob(dd, parent)
{
    kDebug();
}

RequestTextChatJob::~RequestTextChatJob()
{
    kDebug();
}

RequestTextChatJobPrivate::RequestTextChatJobPrivate(const Nepomuk::PersonContact& c,
                                                     const QString ph)
    : RequestChannelJobPrivate(c, QVariantMap(), ph)
{
    kDebug();
}

RequestTextChatJobPrivate::RequestTextChatJobPrivate(const Nepomuk::Person& mc,
                                                     const QString ph)
    : RequestChannelJobPrivate(mc, QVariantMap(), ph)
{
    kDebug();
}

RequestTextChatJobPrivate::~RequestTextChatJobPrivate()
{
    kDebug();
}

Tp::PendingChannelRequest* RequestTextChatJob::ensureChannel()
{
    Q_D(RequestTextChatJob);
    kDebug();
    kDebug() << d->contactResource.resourceUri() << d->contact->alias();
    kDebug() << d->useractiontime;
    kDebug() << d->preferredHandler;
    return d->account->ensureTextChat(d->contact, d->useractiontime, d->preferredHandler);
}

Tp::PendingChannelRequest* RequestTextChatJob::createChannel()
{
    kWarning() << "RequestModeCreate not supported, using RequestModeEnsure";
    return ensureChannel();
}

RequestTextChatJob* requestTextChat(const Nepomuk::PersonContact& contact,
                                    const QString preferredHandler,
                                    QObject* parent)
{
    kDebug() << contact.genericLabel();
    kDebug() << preferredHandler;
    return RequestTextChatJobPrivate::newJob(contact,
                                             preferredHandler,
                                             parent);
}
RequestTextChatJob* requestTextChat(const Nepomuk::Person& metacontact,
                                    const QString preferredHandler,
                                    QObject* parent)
{
    kDebug() << metacontact.genericLabel();
    kDebug() << preferredHandler;
    return RequestTextChatJobPrivate::newJob(metacontact,
                                             preferredHandler,
                                             parent);
}

#include "requesttextchatjob.moc"
