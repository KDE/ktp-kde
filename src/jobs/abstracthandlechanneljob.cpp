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

#include "abstracthandlechanneljob.h"
#include "abstracthandlechanneljob_p.h"

#include "telepathy-bridge.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"

#include <KDebug>
#include <KLocalizedString>

#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingReady>


using namespace KTelepathy;

AbstractHandleChannelJob::AbstractHandleChannelJob(AbstractHandleChannelJobPrivate &dd, QObject *parent)
    : TelepathyBaseJob(dd, parent)
{
    setCapabilities(KJob::Killable);
    connect(this, SIGNAL(handleChannelFinished()), SLOT(onHandleChannelFinished()));
}


AbstractHandleChannelJob::~AbstractHandleChannelJob() {}


void AbstractHandleChannelJob::start()
{
    QTimer::singleShot(0, this, SLOT(__k__handleChannel()));
}


bool AbstractHandleChannelJob::kill(KJob::KillVerbosity verbosity) //TODO verbosity
{
    Q_UNUSED(verbosity)
//    Q_D(AbstractHandleChannelJob);
//    if(d->pendingoperation) //TODO
//        d->pendingoperation->cancel();
    return true;
}


Tp::PendingOperation* AbstractHandleChannelJob::handleIncomingChannel()
{
    kWarning() << "This method should not be called.";
    setError(KJob::UserDefinedError);
    setErrorText(i18n("This is an internal error of KTelepathy"));
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    return NULL;
}


Tp::PendingOperation* AbstractHandleChannelJob::handleOutgoingChannel()
{
    kWarning() << "This method should not be called.";
    setError(KJob::UserDefinedError);
    setErrorText(i18n("This is an internal error of KTelepathy"));
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    return NULL;
}


bool AbstractHandleChannelJob::canHandleIncomingChannel()
{
    return false;
}


bool AbstractHandleChannelJob::canHandleOutgoingChannel()
{
    return false;
}


Tp::Features AbstractHandleChannelJob::channelFeatures()
{
    return Tp::Features();
}


void AbstractHandleChannelJob::onHandleChannelFinished()
{
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
}


AbstractHandleChannelJobPrivate::AbstractHandleChannelJobPrivate(const Tp::ChannelPtr& c,
                                                                 const KTelepathy::HandleChannelFlags hcf)
    : TelepathyBaseJobPrivate(),
      channel(c),
      handlechannelflags(hcf),
      pendingoperation(NULL)
{
    kDebug();
}

AbstractHandleChannelJobPrivate::~AbstractHandleChannelJobPrivate()
{
    kDebug();
}

void AbstractHandleChannelJobPrivate::__k__handleChannel()
{
    Q_Q(AbstractHandleChannelJob);
    //Handle some possible errors
    if(!channel) {
        kWarning() << "Channel is not valid";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is not a Channel"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
    }

    if(pendingoperation) {
        kWarning() << "Pending operation already existing! What Happened?";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (!q->canHandleIncomingChannel() && !q->canHandleOutgoingChannel()) {
        kWarning() << "This job is useless!";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    if (!(handlechannelflags & (KTelepathy::HandleChannelIncoming | KTelepathy::HandleChannelOutgoing))) {
        kWarning() << "What do you whant to handle?";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    requested = channel->immutableProperties()[QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".Requested")].toBool();

    if(!requested) {
        if ( !(handlechannelflags & KTelepathy::HandleChannelIncoming) ) {
            kWarning() << "I'm not supposed to handle incoming channels.";
            q->setError(KJob::UserDefinedError);
            q->setErrorText(i18n("This job is not supposed to handle incoming channels."));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            return;
        }
        if ( !q->canHandleIncomingChannel() ) {
            kWarning() << "This job cannot handle incoming channels.";
            q->setError(KJob::UserDefinedError);
            q->setErrorText(i18n("This job cannot handle incoming channels."));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            return;
        }
    } else {
        if ( !(handlechannelflags & KTelepathy::HandleChannelOutgoing) ) {
            kWarning() << "I'm not supposed to handle outgoing channels.";
            q->setError(KJob::UserDefinedError);
            q->setErrorText(i18n("This job is not supposed to outgoing channels."));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            return;
        }
        if ( !q->canHandleOutgoingChannel() ) {
            kWarning() << "This job cannot handle outgoing channels.";
            q->setError(KJob::UserDefinedError);
            q->setErrorText(i18n("This job cannot handle outgoing channels."));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            return;
        }
    }

    // Sets the features and waits for channel to become ready
    Tp::PendingReady* op = channel->becomeReady(q->channelFeatures());
    q->connect(op, SIGNAL(finished(Tp::PendingOperation*)), SLOT(__k__onChannelReady(Tp::PendingOperation*)));
}


void AbstractHandleChannelJobPrivate::__k__onChannelReady(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(AbstractHandleChannelJob);

    if (op->isError()) {
        kWarning() << "Unable to make the channel ready" << "-" << op->errorName() << ":" << op->errorMessage();
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("Unable to make the channel ready."));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    kDebug() << "Channel ready!";
    Q_EMIT q->infoMessage(q, i18n("Channel ready!"));

    if (!requested) {
        pendingoperation = q->handleIncomingChannel();
    } else {
        pendingoperation = q->handleOutgoingChannel();
    }

    // Check if the pending operation was created...
    if(!pendingoperation) {
        kWarning() << "Pending operation was not created";
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("This is an internal error of KTelepathy"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    q->connect(pendingoperation,
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onHandleChannelFinished(Tp::PendingOperation*)));
}


void AbstractHandleChannelJobPrivate::__k__onHandleChannelFinished( Tp::PendingOperation* op )
{
    Q_Q(AbstractHandleChannelJob);
    if (op->isError()) {
        kWarning() << "Channel handling failed:"
                   << op->errorName()
                   << op->errorMessage();
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("An error occurred handling the channel."));
    }

    //TODO methods to get nepomuk resources from the channel

    Q_EMIT q->handleChannelFinished();
}


#include "abstracthandlechanneljob.moc"
