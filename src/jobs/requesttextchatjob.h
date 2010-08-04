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

#ifndef LIBKTELEPATHY_REQUESTTEXTCHATJOB_H
#define LIBKTELEPATHY_REQUESTTEXTCHATJOB_H


#include "requestchanneljob.h"
#include <kdemacros.h>

class RequestTextChatJobPrivate;
class KDE_EXPORT RequestTextChatJob : public RequestChannelJob
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestTextChatJob)
    Q_DECLARE_PRIVATE(RequestTextChatJob)

public:
    RequestTextChatJob(RequestTextChatJobPrivate &dd, QObject *parent = 0);
    virtual ~RequestTextChatJob();

    virtual Tp::PendingChannelRequest* ensureChannel();
    virtual bool canCreateChannel();
};

/*
KDE_EXPORT RequestTextChatJob* requestTextChat(const Nepomuk::PersonContact& contact,
                                               const QString preferredHandler = QString(),
                                               const RequestChannelJob::RequestFlags requestflags = RequestChannelJob::RequestModeEnsure,
                                               QObject* parent = 0);
KDE_EXPORT RequestTextChatJob* requestTextChat(const Nepomuk::Person& metacontact,
                                               const QString preferredHandler = QString(),
                                               const RequestChannelJob::RequestFlags requestflags = RequestChannelJob::RequestModeEnsure,
                                               QObject* parent = 0);
*/
KDE_EXPORT RequestTextChatJob* requestTextChat(const Nepomuk::PersonContact& contact,
                                               const QString preferredHandler,
                                               QObject* parent);
KDE_EXPORT RequestTextChatJob* requestTextChat(const Nepomuk::Person& metacontact,
                                               const QString preferredHandler,
                                               QObject* parent);
#endif // LIBKTELEPATHY_REQUESTTEXTCHATJOB_H
