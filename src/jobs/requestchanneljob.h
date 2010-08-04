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

#ifndef LIBKTELEPATHY_REQUESTCHANNELJOB_H
#define LIBKTELEPATHY_REQUESTCHANNELJOB_H

#include "telepathy-base-job.h"
#include <kdemacros.h>

namespace Tp {
    class PendingOperation;
    class PendingChannelRequest;
}

class RequestChannelJobPrivate;
class KDE_EXPORT RequestChannelJob : public TelepathyBaseJob
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestChannelJob)
    Q_DECLARE_PRIVATE(RequestChannelJob)

public:
    enum RequestFlag { //TODO change to RequestFlags and use in constructor
        RequestModeNoOption = 0,
        RequestModeEnsure   = 1 << 0,
        RequestModeCreate   = 1 << 1,
    };
    Q_DECLARE_FLAGS(RequestFlags, RequestFlag)

private:
    enum TargetMode {
        TargetModeError = 0,
        TargetModeContact,
        TargetModeMetaContact,
        TargetModeRoom
        //todo ChatRoom ContactList
    };


    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__requestChannel())

    Q_PRIVATE_SLOT(d_func(), void __k__onPendingChannelRequestFinished(Tp::PendingOperation* op))

    Q_FLAGS(RequestFlags)

protected:
    RequestChannelJob(RequestChannelJobPrivate &dd, QObject *parent = 0);
    virtual ~RequestChannelJob();

    virtual Tp::PendingChannelRequest* ensureChannel();
    virtual Tp::PendingChannelRequest* createChannel();
    virtual bool canEnsureChannel();
    virtual bool canCreateChannel();
    virtual void onChannelRequestFinished(Tp::PendingOperation* op);

public:
    virtual void start();
    virtual bool kill(KJob::KillVerbosity verbosity = KJob::Quietly);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RequestChannelJob::RequestFlags)


#include <QVariantMap>
namespace Nepomuk {
    class PersonContact;
    class Person;
}


KDE_EXPORT RequestChannelJob* requestChannel(const Nepomuk::PersonContact& contact,
                                             const QVariantMap& request,
                                             const QString preferredHandler = QString(),
                                             RequestChannelJob::RequestFlags requestchannelflags = RequestChannelJob::RequestModeEnsure,
                                             QObject* parent = 0);

KDE_EXPORT RequestChannelJob* requestChannel(const Nepomuk::Person& metacontact,
                                             const QVariantMap& request,
                                             const QString preferredHandler = QString(),
                                             RequestChannelJob::RequestFlags requestchannelflags = RequestChannelJob::RequestModeEnsure,
                                             QObject* parent = 0);

KDE_EXPORT RequestChannelJob* requestChannel(const QString room,
                                             const QVariantMap& request,
                                             const QString preferredHandler = QString(),
                                             RequestChannelJob::RequestFlags requestchannelflags = RequestChannelJob::RequestModeEnsure,
                                             QObject* parent = 0);

#endif // LIBKTELEPATHY_REQUESTCHANNELJOB_H
