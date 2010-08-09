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

#ifndef LIBKTELEPATHY_ABSTRACTHANDLECHANNELJOB_H
#define LIBKTELEPATHY_ABSTRACTHANDLECHANNELJOB_H

#include <telepathy-base-job.h>
#include <kdemacros.h>
#include <TelepathyQt4/Feature>

namespace KTelepathy {

    enum HandleChannelFlag {
         HandleChannelNoOption     = 0,
         HandleChannelIncoming     = 1 << 0,
         HandleChannelOutgoing     = 1 << 1,
         HandleChannelAny          = HandleChannelIncoming | HandleChannelOutgoing
    };
    Q_DECLARE_FLAGS(HandleChannelFlags, HandleChannelFlag)
    //Q_FLAGS(HandleChannelFlags) // TODO REMOVE?

} // namespace KTelepathy

Q_DECLARE_OPERATORS_FOR_FLAGS(KTelepathy::HandleChannelFlags)


namespace Tp {
    class PendingOperation;
}

class AbstractHandleChannelJobPrivate;
class KDE_EXPORT AbstractHandleChannelJob : public TelepathyBaseJob // TODO Remove KDE_EXPORT?
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractHandleChannelJob)
    Q_DECLARE_PRIVATE(AbstractHandleChannelJob)

protected:
    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__handleChannel())
    Q_PRIVATE_SLOT(d_func(), void __k__onChannelReady(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onHandleChannelFinished( Tp::PendingOperation* op ))

    AbstractHandleChannelJob(AbstractHandleChannelJobPrivate &dd, QObject *parent = 0);
    virtual ~AbstractHandleChannelJob();

    virtual Tp::PendingOperation* handleIncomingChannel();
    virtual Tp::PendingOperation* handleOutgoingChannel();
    virtual bool canHandleIncomingChannel();
    virtual bool canHandleOutgoingChannel();
    virtual Tp::Features features(); // TODO choose a better name

protected Q_SLOTS:
    virtual void onHandleChannelFinished();

Q_SIGNALS:
    void handleChannelFinished();

public:
    virtual void start();
    virtual bool kill(KJob::KillVerbosity verbosity = KJob::Quietly);
};

#endif // ABSTRACTHANDLECHANNELJOB_H
