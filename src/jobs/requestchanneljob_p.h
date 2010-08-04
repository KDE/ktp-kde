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

#include "telepathy-base-job_p.h"
#include "requestchanneljob.h"

#ifndef LIBKTELEPATHY_REQUESTCHANNELJOB_P_H
#define LIBKTELEPATHY_REQUESTCHANNELJOB_P_H

#include "ontologies/person.h"
#include "ontologies/personcontact.h"
#include <QDateTime>
#include <TelepathyQt4/Types>

namespace Tp {
    class PendingChannelRequest;
}

class RequestChannelJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(RequestChannelJob)

public:
    RequestChannelJobPrivate(const Nepomuk::PersonContact& c,
                             const QVariantMap& rq,
                             const QString ph,
                             const RequestChannelJob::RequestFlags f);
    RequestChannelJobPrivate(const Nepomuk::Person& mc,
                             const QVariantMap& rq,
                             const QString ph,
                             const RequestChannelJob::RequestFlags f);
    RequestChannelJobPrivate(const QString r,
                             const QVariantMap& requestq,
                             const QString ph,
                             const RequestChannelJob::RequestFlags f);
/* TODO
    RequestChannelJobPrivate(const QString r,
                             const QList<Nepomuk::PersonContact>& cl,
                             const QVariantMap& rq,
                             const QString ph,
                             const RequestChannelJob::RequestFlags f);

    RequestChannelJobPrivate(const QString r,
                             const QList<Nepomuk::Person>& mcl,
                             const QVariantMap& rq,
                             const QString ph,
                             const RequestChannelJob::RequestFlags f);
*/

    virtual ~RequestChannelJobPrivate();

    RequestChannelJob::TargetMode targetmode;
    RequestChannelJob::RequestFlags requestflags;

    Nepomuk::PersonContact contactResource;
    Nepomuk::Person metacontactResource;
    QString room;
//    QList<Nepomuk::PersonContact> contactResources;
//    QList<Nepomuk::Person> metacontactResources;

    QVariantMap request;
    QDateTime useractiontime;
    QString preferredHandler;

    Tp::PendingChannelRequest* m_pendingChannelRequest;

    Tp::AccountPtr account;
    Tp::ContactPtr contact;

    // Operation Q_PRIVATE_SLOTS
    void __k__requestChannel();

    void __k__onPendingChannelRequestFinished(Tp::PendingOperation* op);

protected:
    void updateRequestTargets();

private:
    void initTargets();
    void initTargetsModeContact();
    void initTargetsModeMetaContact();

public:
    static inline RequestChannelJob* newJob(const Nepomuk::PersonContact& contact,
                                            const QVariantMap& request,
                                            const QString preferredHandler,
                                            const RequestChannelJob::RequestFlags flags,
                                            QObject* parent)
    {
        RequestChannelJob *job = new RequestChannelJob(*new RequestChannelJobPrivate(contact, request, preferredHandler, flags), parent);
//TODO?        job->setUiDelegate(new JobUiDelegate);
        return job;
    }

    static inline RequestChannelJob* newJob(const Nepomuk::Person& metacontact,
                                            const QVariantMap& request,
                                            const QString preferredHandler,
                                            const RequestChannelJob::RequestFlags flags,
                                            QObject* parent)
    {
        RequestChannelJob *job = new RequestChannelJob(*new RequestChannelJobPrivate(metacontact, request, preferredHandler, flags), parent);
        return job;
    }

    static inline RequestChannelJob* newJob(const QString room,
                                            const QVariantMap& request,
                                            const QString preferredHandler,
                                            RequestChannelJob::RequestFlags flags,
                                            QObject* parent)
    {
        RequestChannelJob *job = new RequestChannelJob(*new RequestChannelJobPrivate(room, request, preferredHandler, flags), parent);
        return job;
    }

};


#endif // LIBKTELEPATHY_REQUESTCHANNELJOB_H
