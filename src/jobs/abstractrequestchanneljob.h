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

#ifndef LIBKTELEPATHY_ABSTRACTREQUESTCHANNELJOB_H
#define LIBKTELEPATHY_ABSTRACTREQUESTCHANNELJOB_H

#include <telepathy-base-job.h>
#include <kdemacros.h>


namespace Tp {
    class PendingOperation;
    class PendingChannelRequest;
}

namespace KTelepathy {

    enum RequestChannelFlag {
         RequestChannelNoOption     = 0,
         RequestChannelEnsureMode   = 1 << 0,
         RequestChannelCreateMode   = 1 << 1,
         RequestChannelAnyMode      = RequestChannelEnsureMode | RequestChannelCreateMode
    };
    Q_DECLARE_FLAGS(RequestChannelFlags, RequestChannelFlag)
    //Q_FLAGS(RequestChannelFlags) //TODO REMOVE?


class AbstractRequestChannelJobPrivate;
class KDE_EXPORT AbstractRequestChannelJob : public TelepathyBaseJob // TODO Remove KDE_EXPORT?
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractRequestChannelJob)
    Q_DECLARE_PRIVATE(AbstractRequestChannelJob)

protected:
    enum TargetMode {
         TargetModeError = 0,
         TargetModeContact,
         TargetModeMetaContact,
         TargetModeRoom,
         TargetModeRoomContactsList,
         TargetModeRoomMetaContactsList
    };

    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__requestChannel())
    Q_PRIVATE_SLOT(d_func(), void __k__onRequestChannelFinished(Tp::PendingOperation* op))

    AbstractRequestChannelJob(AbstractRequestChannelJobPrivate &dd, QObject *parent = 0);
    virtual ~AbstractRequestChannelJob();

    virtual Tp::PendingChannelRequest* ensureChannel();
    virtual Tp::PendingChannelRequest* createChannel();
    virtual bool canEnsureChannel();
    virtual bool canCreateChannel();

protected Q_SLOTS:
    virtual void onRequestChannelFinished();

public:
    virtual void start();
    virtual bool kill(KJob::KillVerbosity verbosity = KJob::Quietly);

Q_SIGNALS:
    void requestChannelFinished();

};

} // namespace KTelepathy

Q_DECLARE_OPERATORS_FOR_FLAGS(KTelepathy::RequestChannelFlags)


#endif // LIBKTELEPATHY_ABSTRACTREQUESTCHANNELJOB_H
