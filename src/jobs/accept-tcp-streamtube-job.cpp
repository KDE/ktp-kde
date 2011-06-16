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

#include "accept-tcp-streamtube-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"

#include <QTimer>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/IncomingStreamTubeChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingOperation>


namespace KTelepathy {

class AcceptTcpStreamTubeJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(AcceptTcpStreamTubeJob)

    public:
        AcceptTcpStreamTubeJobPrivate(const QVariantMap& p, const QHostAddress& h);
        virtual ~AcceptTcpStreamTubeJobPrivate();

        Tp::IncomingStreamTubeChannelPtr channel;

        const QVariantMap& parameters;
        quint16 allowedPort;
        const QHostAddress& hostAddress;
        QTcpSocket* socket;
        QIODevice* device;

        void __k__acceptTcpStreamTube();
        void __k__onStreamTubeChannelReady(Tp::PendingOperation* op);
        void __k__onStreamTubeAccepted(Tp::PendingOperation* op);
        void __k__onInvalidated();
};

} // namespace KTelepathy

using namespace KTelepathy;

/* TODO?
AcceptTcpStreamTubeJob::AcceptTcpStreamTubeJob(const QHostAddress& allowedAddress,
                                               quint16 allowedPort,
                                               const QVariantMap& parameters,
                                               QObject* parent)
    : TelepathyBaseJob(*new AcceptTcpStreamTubeJobPrivate(this, parameters, allowedAddress), parent)
{
    Q_D(AcceptTcpStreamTubeJob);

    d->allowedPort = allowedPort;
}
*/

AcceptTcpStreamTubeJob::AcceptTcpStreamTubeJob(Tp::ChannelPtr channel,
                                               const QHostAddress& allowedAddress,
                                               quint16 allowedPort,
                                               const QVariantMap& parameters,
                                               QObject* parent)
    : TelepathyBaseJob(*new AcceptTcpStreamTubeJobPrivate(parameters, allowedAddress), parent)
{
    Q_D(AcceptTcpStreamTubeJob);

    d->allowedPort = allowedPort;
    d->channel = Tp::IncomingStreamTubeChannelPtr::dynamicCast(channel); //TODO check if != 0
}

AcceptTcpStreamTubeJob::~AcceptTcpStreamTubeJob() {}


void AcceptTcpStreamTubeJob::start()
{
    QTimer::singleShot(0, this, SLOT(__k__acceptTcpStreamTube()));
}


QTcpSocket* AcceptTcpStreamTubeJob::tcpSocket()
{
    Q_D(AcceptTcpStreamTubeJob);
    return d->socket;
}


QIODevice* AcceptTcpStreamTubeJob::device()
{
    Q_D(AcceptTcpStreamTubeJob);
    return d->device;
}

Tp::IncomingStreamTubeChannelPtr AcceptTcpStreamTubeJob::incomingStreamTubeChannel()
{
    Q_D(AcceptTcpStreamTubeJob);
    return d->channel;
}


AcceptTcpStreamTubeJobPrivate::AcceptTcpStreamTubeJobPrivate(const QVariantMap& p, const QHostAddress& h)
    : parameters(p),
      hostAddress(h),
      socket(NULL)
{
}


AcceptTcpStreamTubeJobPrivate::~AcceptTcpStreamTubeJobPrivate() {}



void AcceptTcpStreamTubeJobPrivate::__k__acceptTcpStreamTube()
{
    kDebug();
    Q_Q(AcceptTcpStreamTubeJob);

    Tp::Features features = Tp::Features() << Tp::TubeChannel::FeatureTube
                                           << Tp::StreamTubeChannel::FeatureStreamTube;
    Tp::PendingReady* op = channel->becomeReady(features);
    q->connect(op, SIGNAL(finished(Tp::PendingOperation *)),
               q, SLOT(__k__onStreamTubeChannelReady(Tp::PendingOperation*)));
}


void AcceptTcpStreamTubeJobPrivate::__k__onStreamTubeChannelReady(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(AcceptTcpStreamTubeJob);

    if (op->isError()) {
        kWarning() << "Unable to make stream tube channel ready -" <<
            op->errorName() << ":" << op->errorMessage();
        __k__onInvalidated();
        return;
    }

    kDebug() << "Stream tube channel ready!";
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel ready."));

    q->connect(channel->acceptTubeAsTcpSocket(),
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onStreamTubeAccepted(Tp::PendingOperation*)));
}


void AcceptTcpStreamTubeJobPrivate::__k__onStreamTubeAccepted(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(AcceptTcpStreamTubeJob);

    if (op->isError()) {
        qWarning() << "Unable to accept stream tube channel -" <<
            op->errorName() << ":" << op->errorMessage();
        __k__onInvalidated();
        return;
    }

    kDebug() << "Stream tube channel accepted and opened!";
    Q_EMIT q->infoMessage(q, i18n("Stream tube channel accepted and opened."));

    device = channel->device();

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void AcceptTcpStreamTubeJobPrivate::__k__onInvalidated()
{
    Q_Q(AcceptTcpStreamTubeJob);

    kWarning() << "Stream tube invalidated!";
    Q_EMIT q->infoMessage(q, i18n("Stream tube invalidated."));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


#include "accept-tcp-streamtube-job.moc"
