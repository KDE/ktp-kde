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

#ifndef LIBKTELEPATHY_ABSTRACTHANDLECHANNELJOB_P_H
#define LIBKTELEPATHY_ABSTRACTHANDLECHANNELJOB_P_H

#include "telepathy-base-job_p.h"
#include "abstracthandlechanneljob.h"

#include <TelepathyQt4/Channel>


class AbstractHandleChannelJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(AbstractHandleChannelJob)

public:
    AbstractHandleChannelJobPrivate(const Tp::ChannelPtr& c,
                                    const KTelepathy::HandleChannelFlags hcf);
    virtual ~AbstractHandleChannelJobPrivate();

    const Tp::ChannelPtr channel;
    const KTelepathy::HandleChannelFlags handlechannelflags;

    // Handle operation
    Tp::PendingOperation* pendingoperation;

    bool requested;

    // Operation Q_PRIVATE_SLOTS
    void __k__handleChannel();
    void __k__onChannelReady(Tp::PendingOperation* op);
    void __k__onHandleChannelFinished( Tp::PendingOperation* op );

private:
    //TODO methods to get nepomuk resources from the channel
};

#endif // LIBKTELEPATHY_ABSTRACTHANDLECHANNELJOB_P_H
