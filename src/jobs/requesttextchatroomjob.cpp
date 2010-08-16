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

#include "requesttextchatroomjob.h"
#include "abstractrequestchanneljob_p.h"

#include <TelepathyQt4/Account>
#include <KDebug>

class RequestTextChatroomJobPrivate : public AbstractRequestChannelJobPrivate
{
    Q_DECLARE_PUBLIC(RequestTextChatroomJob)

public:
    RequestTextChatroomJobPrivate(const QString& r,
                                  const QString& ph,
                                  const KTelepathy::RequestChannelFlags rcf);
    ~RequestTextChatroomJobPrivate();

    static inline RequestTextChatroomJob* newJob(const QString& room,
                                                 const QString& preferredHandler,
                                                 const KTelepathy::RequestChannelFlags requestchannelflags,
                                                 QObject* parent)
    {
        kDebug() << room;
        kDebug() << preferredHandler;
        kDebug() << requestchannelflags;
        RequestTextChatroomJob *job = new RequestTextChatroomJob(*new RequestTextChatroomJobPrivate(room,
                                                                                                    preferredHandler,
                                                                                                    requestchannelflags),
                                                                 parent);
        return job;
    }
};


RequestTextChatroomJob::RequestTextChatroomJob(RequestTextChatroomJobPrivate &dd, QObject *parent)
    : AbstractRequestChannelJob(dd, parent)
{
}


RequestTextChatroomJob::~RequestTextChatroomJob()
{
}


Tp::PendingChannelRequest* RequestTextChatroomJob::ensureChannel()
{
    Q_D(RequestTextChatroomJob);
    kDebug() << d->contactResource.resourceUri() << d->contact->id();
    kDebug() << d->useractiontime;
    kDebug() << d->preferredHandler;
    return d->account->ensureTextChatroom(d->room, d->useractiontime, d->preferredHandler);
}


bool RequestTextChatroomJob::canEnsureChannel()
{
    return true;
}


RequestTextChatroomJobPrivate::RequestTextChatroomJobPrivate(const QString& r,
                                                             const QString& ph,
                                                             const KTelepathy::RequestChannelFlags rcf)
    : AbstractRequestChannelJobPrivate(r, ph, rcf)
{
    kDebug();
}


RequestTextChatroomJobPrivate::~RequestTextChatroomJobPrivate()
{
    kDebug();
}

/*
RequestTextChatroomJob* requestTextChatroom(const QString& room,
                                            const QString& preferredHandler,
                                            const KTelepathy::RequestChannelFlags requestchannelflags,
                                            QObject* parent)
{
    return RequestTextChatroomJobPrivate::newJob(room,
                                                 preferredHandler,
                                                 requestchannelflags,
                                                 parent);
}
*/

RequestTextChatroomJob* requestTextChatroom(const QString& room,
                                            const QString& preferredHandler,
                                            QObject* parent)
{
    kDebug();
    return RequestTextChatroomJobPrivate::newJob(room,
                                                 preferredHandler,
                                                 KTelepathy::RequestChannelEnsureMode,
                                                 parent);
}


#include "requesttextchatroomjob.moc"
