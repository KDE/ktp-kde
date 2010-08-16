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

#ifndef LIBKTELEPATHY_REQUESTTEXTCHATROOMJOB_H
#define LIBKTELEPATHY_REQUESTTEXTCHATROOMJOB_H


#include <abstractrequestchanneljob.h>
#include <kdemacros.h>

namespace KTelepathy {

class RequestTextChatroomJobPrivate;
class KDE_EXPORT RequestTextChatroomJob : public AbstractRequestChannelJob
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestTextChatroomJob)
    Q_DECLARE_PRIVATE(RequestTextChatroomJob)

public:
    RequestTextChatroomJob(RequestTextChatroomJobPrivate &dd, QObject *parent = 0);
    virtual ~RequestTextChatroomJob();

    virtual Tp::PendingChannelRequest* ensureChannel();
    virtual bool canEnsureChannel();
};


/*
KDE_EXPORT RequestTextChatroomJob* requestTextChatroom(const QString& room,
                                                       const QString& preferredHandler = QString(),
                                                       const RequestChannelJob::RequestFlags requestchannelflags = RequestChannelJob::RequestModeEnsure,
                                                       QObject* parent = 0);
*/
KDE_EXPORT RequestTextChatroomJob* requestTextChatroom(const QString& room,
                                                       const QString& preferredHandler,
                                                       QObject* parent);

} // namespace KTelepathy

#endif // LIBKTELEPATHY_REQUESTTEXTCHATROOMJOB_H
