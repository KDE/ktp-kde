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

#ifndef LIBKTELEPATHY_HANDLEP2PDBUSTUBEJOB_H
#define LIBKTELEPATHY_HANDLEP2PDBUSTUBEJOB_H

#include <kdemacros.h>

#include "abstracthandlechanneljob.h"

#include <QVariantMap>
#include <TelepathyQt4/Channel>


namespace Tp {
    class DBusTubeChannel;
}

class QDBusConnection;


namespace KTelepathy {

class HandleP2PDBusTubeJobPrivate;
class KDE_EXPORT HandleP2PDBusTubeJob : public AbstractHandleChannelJob
{
    Q_OBJECT
    Q_DISABLE_COPY(HandleP2PDBusTubeJob)
    Q_DECLARE_PRIVATE(HandleP2PDBusTubeJob)

protected:
    HandleP2PDBusTubeJob(HandleP2PDBusTubeJobPrivate& dd, QObject* parent = 0);
    virtual ~HandleP2PDBusTubeJob();

    virtual Tp::PendingOperation* handleIncomingChannel();
    virtual Tp::PendingOperation* handleOutgoingChannel();
    virtual bool canHandleIncomingChannel();
    virtual bool canHandleOutgoingChannel();
    virtual Tp::Features channelFeatures();

protected Q_SLOTS:
    virtual void onHandleChannelFinished();

public:
    QDBusConnection dbusConnection();
//    Tp::IncomingDBusTubeChannelPtr incomingP2PDBusTubeChannel(); //TODO Add?
//    Tp::OutgoingDBusTubeChannelPtr outgoingP2PDBusTubeChannel(); //TODO Add?
};



KDE_EXPORT HandleP2PDBusTubeJob* handleP2PDBusTube(const Tp::ChannelPtr& channel,
                                                   const QVariantMap& parameters,
                                                   const bool requireCredentials = false,
                                                   const KTelepathy::HandleChannelFlags handlechannelflags = KTelepathy::HandleChannelAny,
                                                   QObject* parent = 0);
KDE_EXPORT HandleP2PDBusTubeJob* handleP2PDBusTube(const Tp::ChannelPtr& channel,
                                                   const bool requireCredentials,
                                                   const KTelepathy::HandleChannelFlags handlechannelflags = KTelepathy::HandleChannelAny,
                                                   QObject* parent = 0);

KDE_EXPORT Tp::ChannelClassList P2PDBusTubeClientHandlerChannelClassList(QString serviceName);

} // namespace KTelepathy

#endif // LIBKTELEPATHY_HANDLEP2PDBUSTUBEJOB_H
