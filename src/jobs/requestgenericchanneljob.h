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

#ifndef REQUESTGENERICCHANNELJOB_H
#define REQUESTGENERICCHANNELJOB_H

#include "abstractrequestchanneljob.h"
#include <kdemacros.h>
#include <QVariantMap>


namespace Nepomuk {
    class PersonContact;
    class Person;
    class IMAccount;
}

namespace KTelepathy {

class RequestGenericChannelJobPrivate;
class KDE_EXPORT RequestGenericChannelJob : public AbstractRequestChannelJob
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestGenericChannelJob)
    Q_DECLARE_PRIVATE(RequestGenericChannelJob)

protected:
    RequestGenericChannelJob(RequestGenericChannelJobPrivate &dd, QObject *parent = 0);
    virtual ~RequestGenericChannelJob();

    virtual Tp::PendingChannelRequest* ensureChannel();
    virtual Tp::PendingChannelRequest* createChannel();
    virtual bool canEnsureChannel();
    virtual bool canCreateChannel();

    void updateRequestTargets();
};



KDE_EXPORT RequestGenericChannelJob* requestGenericChannel(const Nepomuk::PersonContact& contact,
                                                           const QVariantMap& request,
                                                           const QString& preferredHandler = QString(),
                                                           KTelepathy::RequestChannelFlags requestchannelflags = KTelepathy::RequestChannelAnyMode,
                                                           QObject* parent = 0);

KDE_EXPORT RequestGenericChannelJob* requestGenericChannel(const Nepomuk::Person& metacontact,
                                                           const QVariantMap& request,
                                                           const QString& preferredHandler = QString(),
                                                           KTelepathy::RequestChannelFlags requestchannelflags = KTelepathy::RequestChannelAnyMode,
                                                           QObject* parent = 0);

KDE_EXPORT RequestGenericChannelJob* requestGenericChannel(const QString room,
                                                           const QVariantMap& request,
                                                           const QString& preferredHandler = QString(),
                                                           KTelepathy::RequestChannelFlags requestchannelflags = KTelepathy::RequestChannelAnyMode,
                                                           QObject* parent = 0);

KDE_EXPORT RequestGenericChannelJob* requestGenericChannel(const Nepomuk::IMAccount& account,
                                                           const QString room,
                                                           const QVariantMap& request,
                                                           const QString& preferredHandler = QString(),
                                                           KTelepathy::RequestChannelFlags requestchannelflags = KTelepathy::RequestChannelAnyMode,
                                                           QObject* parent = 0);

} // namespace KTelepathy


#endif // REQUESTGENERICCHANNELJOB_H
