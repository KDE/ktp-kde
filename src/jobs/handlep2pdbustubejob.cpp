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

#include "handlep2pdbustubejob.h"
#include "abstracthandlechanneljob_p.h"

#include <QTimer>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/IncomingDBusTubeChannel>
#include <TelepathyQt4/OutgoingDBusTubeChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingOperation>



class HandleP2PDBusTubeJobPrivate : public AbstractHandleChannelJobPrivate
{
    Q_DECLARE_PUBLIC(HandleP2PDBusTubeJob)

public:
    HandleP2PDBusTubeJobPrivate(const Tp::ChannelPtr& c,
                                const QVariantMap& p,
                                const bool r,
                                const KTelepathy::HandleChannelFlags hcf);
    virtual ~HandleP2PDBusTubeJobPrivate();

    Tp::IncomingDBusTubeChannelPtr inChannel;
    Tp::OutgoingDBusTubeChannelPtr outChannel;

    const QVariantMap parameters;
    bool requireCredentials;
    QDBusConnection dbusconnection;

    static inline HandleP2PDBusTubeJob* newJob(const Tp::ChannelPtr& channel,
                                               const QVariantMap& parameters,
                                               const bool requireCredentials,
                                               const KTelepathy::HandleChannelFlags handlechannelflags,
                                               QObject* parent)
    {
        kDebug() << channel;
        kDebug() << parameters;
        kDebug() << requireCredentials;
        HandleP2PDBusTubeJob *job = new HandleP2PDBusTubeJob(*new HandleP2PDBusTubeJobPrivate(channel,
                                                                                              parameters,
                                                                                              requireCredentials,
                                                                                              handlechannelflags),
                                                             parent);
        return job;
    }
    static inline HandleP2PDBusTubeJob* newJob(const Tp::ChannelPtr& channel,
                                               const bool requireCredentials,
                                               const KTelepathy::HandleChannelFlags handlechannelflags,
                                               QObject* parent)
    {
        kDebug() << channel;
        kDebug() << requireCredentials;
        HandleP2PDBusTubeJob *job = new HandleP2PDBusTubeJob(*new HandleP2PDBusTubeJobPrivate(channel,
                                                                                              QVariantMap(),
                                                                                              requireCredentials,
                                                                                              handlechannelflags),
                                                             parent);
        return job;
    }
};


HandleP2PDBusTubeJob::HandleP2PDBusTubeJob(HandleP2PDBusTubeJobPrivate& dd, QObject* parent)
    : AbstractHandleChannelJob(dd, parent)
{
}


HandleP2PDBusTubeJob::~HandleP2PDBusTubeJob()
{
}


Tp::PendingOperation* HandleP2PDBusTubeJob::handleIncomingChannel()
{
    Q_D(HandleP2PDBusTubeJob);
    d->inChannel = Tp::IncomingDBusTubeChannelPtr::dynamicCast(d->channel);
    if (!d->inChannel) {
        kWarning() << "This is not an incoming dbus tube channel";
        setError(KJob::UserDefinedError);
        setErrorText(i18n("Unable to make the channel ready."));
        QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    }
    return d->inChannel->acceptTube(d->requireCredentials);
}


Tp::PendingOperation* HandleP2PDBusTubeJob::handleOutgoingChannel()
{
    Q_D(HandleP2PDBusTubeJob);
    d->outChannel = Tp::OutgoingDBusTubeChannelPtr::dynamicCast(d->channel);
    if (!d->outChannel) {
        kWarning() << "This is not an outgoing dbus tube channel";
        setError(KJob::UserDefinedError);
        setErrorText(i18n("Unable to make the channel ready."));
        QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    }
    return d->outChannel->offerTube(d->parameters, d->requireCredentials);
}


bool HandleP2PDBusTubeJob::canHandleIncomingChannel()
{
    return true;
}


bool HandleP2PDBusTubeJob::canHandleOutgoingChannel()
{
    return true;
}


Tp::Features HandleP2PDBusTubeJob::channelFeatures()
{
    Tp::Features features = Tp::Features() << Tp::TubeChannel::FeatureTube
                                           << Tp::DBusTubeChannel::FeatureDBusTube;
    return features;
}


void HandleP2PDBusTubeJob::onHandleChannelFinished()
{
    Q_D(HandleP2PDBusTubeJob);
    if(d->inChannel)
        d->dbusconnection = d->inChannel->connection();
    else if (d->outChannel)
        d->dbusconnection = d->outChannel->connection();

    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
}



QDBusConnection HandleP2PDBusTubeJob::dbusConnection()
{
    Q_D(HandleP2PDBusTubeJob);
    return d->dbusconnection;
}


// TODO Add?
/*
Tp::IncomingDBusTubeChannelPtr HandleP2PDBusTubeJob::incomingDBusTubeChannel()
{
    Q_D(HandleP2PDBusTubeJob);
    return d->inChannel;
}


Tp::OutgoingDBusTubeChannelPtr HandleP2PDBusTubeJob::outgoingDBusTubeChannel()
{
    Q_D(HandleP2PDBusTubeJob);
    return d->outChannel;
}
*/


HandleP2PDBusTubeJobPrivate::HandleP2PDBusTubeJobPrivate(const Tp::ChannelPtr& c,
                                                         const QVariantMap& p,
                                                         const bool r,
                                                         const KTelepathy::HandleChannelFlags hcf)
    : AbstractHandleChannelJobPrivate(c, hcf),
      parameters(p),
      requireCredentials(r),
      dbusconnection(QLatin1String("none"))
{
}


HandleP2PDBusTubeJobPrivate::~HandleP2PDBusTubeJobPrivate()
{
}


HandleP2PDBusTubeJob* handleP2PDBusTube(const Tp::ChannelPtr& channel,
                                        const QVariantMap& parameters,
                                        const bool requireCredentials,
                                        const KTelepathy::HandleChannelFlags handlechannelflags,
                                        QObject* parent)
{
    kDebug();
    return HandleP2PDBusTubeJobPrivate::newJob(channel,
                                               parameters,
                                               requireCredentials,
                                               handlechannelflags,
                                               parent);
}

HandleP2PDBusTubeJob* handleP2PDBusTube(const Tp::ChannelPtr& channel,
                                        const bool requireCredentials,
                                        const KTelepathy::HandleChannelFlags handlechannelflags,
                                        QObject* parent)
{
    kDebug();
    return HandleP2PDBusTubeJobPrivate::newJob(channel,
                                               QVariantMap(),
                                               requireCredentials,
                                               handlechannelflags,
                                               parent);
}


Tp::ChannelClassList P2PDBusTubeClientHandlerChannelClassList(QString serviceName)
{
    QMap<QString, QDBusVariant> filter0;

    filter0[QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType")] =
                QDBusVariant(QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE));
    filter0[QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_DBUS_TUBE ".ServiceName")] =
                QDBusVariant(serviceName);
    filter0[QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType")] =
                QDBusVariant(1);

    return Tp::ChannelClassList() << Tp::ChannelClass(filter0);
}


#include "handlep2pdbustubejob.moc"
