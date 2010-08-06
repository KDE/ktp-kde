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
#include "abstractrequestchanneljob.h"

#ifndef LIBKTELEPATHY_ABSTRACTREQUESTCHANNELJOB_P_H
#define LIBKTELEPATHY_ABSTRACTREQUESTCHANNELJOB_P_H

#include "ontologies/person.h"
#include "ontologies/personcontact.h"
#include <QDateTime>
#include <TelepathyQt4/Types>

namespace Tp {
    class PendingChannelRequest;
}

class AbstractRequestChannelJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(AbstractRequestChannelJob)

public:
    AbstractRequestChannelJobPrivate(const Nepomuk::PersonContact& c,
                                     const QString& ph,
                                     const KTelepathy::RequestChannelFlags rcf);
    AbstractRequestChannelJobPrivate(const Nepomuk::Person& mc,
                                     const QString& ph,
                                     const KTelepathy::RequestChannelFlags rcf);
    AbstractRequestChannelJobPrivate(const QString& r,
                                     const QString& ph,
                                     const KTelepathy::RequestChannelFlags rcf);
/* TODO
    AbstractRequestChannelJobPrivate(const QString& r,
                                     const QList<Nepomuk::PersonContact>& cl,
                                     const QString& ph,
                                     const KTelepathy::RequestChannelFlags rcf);

    AbstractRequestChannelJobPrivate(const QString& r,
                                     const QList<Nepomuk::Person>& mcl,
                                     const QString& ph,
                                     const KTelepathy::RequestChannelFlags rcf);
*/

    virtual ~AbstractRequestChannelJobPrivate();

    // Flags
    const AbstractRequestChannelJob::TargetMode targetmode;
    const KTelepathy::RequestChannelFlags requestchannelflags;

    // Target (input)
    Nepomuk::PersonContact contactResource; // TODO const?
    const Nepomuk::Person metacontactResource;
    const QString room;
//    const QList<Nepomuk::PersonContact> contactResources;
//    const QList<Nepomuk::Person> metacontactResources;

    // Other input
    const QDateTime useractiontime;
    const QString preferredHandler;

    // TelepathyQt4 stuff
    Tp::AccountPtr account;
    Tp::ContactPtr contact;

    // Channel Request
    Tp::PendingChannelRequest* pendingchannelrequest;

    // Operation Q_PRIVATE_SLOTS
    void __k__requestChannel();
    void __k__onRequestChannelFinished(Tp::PendingOperation* op);

private:
    void initTargets();
    void initTargetsModeContact();
    void initTargetsModeMetaContact();
};


#endif // LIBKTELEPATHY_REQUESTCHANNELJOB_H
