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

#include "accept-dbustube-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"

#include <QTimer>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/IncomingDBusTubeChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingOperation>

namespace KTelepathy {

class AcceptDBusTubeJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(AcceptDBusTubeJob)

    public:
        AcceptDBusTubeJobPrivate(const QVariantMap& p);
        virtual ~AcceptDBusTubeJobPrivate();

        Tp::IncomingDBusTubeChannelPtr channel;

        const QVariantMap& parameters;
        QDBusConnection connection;

        void __k__acceptDBusTube();
        void __k__onDBusTubeChannelReady(Tp::PendingOperation* op);
        void __k__onDBusTubeAccepted(Tp::PendingOperation* op);
        void __k__onInvalidated();
};

} // namespace KTelepathy

using namespace KTelepathy;

/* TODO?
AcceptDBusTubeJob::AcceptDBusTubeJob(const QVariantMap& parameters,
                                     QObject* parent)
    : TelepathyBaseJob(*new AcceptDBusTubeJobPrivate(this, parameters, allowedAddress), parent)
{
    Q_D(AcceptDBusTubeJob);
}
*/
AcceptDBusTubeJob::AcceptDBusTubeJob(Tp::ChannelPtr channel,
                                     const QVariantMap& parameters,
                                     QObject* parent)
    : TelepathyBaseJob(*new AcceptDBusTubeJobPrivate(parameters), parent)
{
    Q_D(AcceptDBusTubeJob);

    d->channel = Tp::IncomingDBusTubeChannelPtr::dynamicCast(channel); //TODO check if != 0
}

AcceptDBusTubeJob::~AcceptDBusTubeJob() {}


void AcceptDBusTubeJob::start()
{
    QTimer::singleShot(0, this, SLOT(__k__acceptDBusTube()));
}


QDBusConnection AcceptDBusTubeJob::dbusConnection()
{
    Q_D(AcceptDBusTubeJob);
    return d->connection;
}


Tp::IncomingDBusTubeChannelPtr AcceptDBusTubeJob::incomingDBusTubeChannel()
{
    Q_D(AcceptDBusTubeJob);
    return d->channel;
}


AcceptDBusTubeJobPrivate::AcceptDBusTubeJobPrivate(const QVariantMap& p)
    : parameters(p),
      connection(QLatin1String("none"))
{
}


AcceptDBusTubeJobPrivate::~AcceptDBusTubeJobPrivate() {}



void AcceptDBusTubeJobPrivate::__k__acceptDBusTube()
{
    kDebug();
    Q_Q(AcceptDBusTubeJob);

    Tp::Features features = Tp::Features() << Tp::TubeChannel::FeatureTube
                                           << Tp::DBusTubeChannel::FeatureDBusTube;
    Tp::PendingReady* op = channel->becomeReady(features);
    q->connect(op, SIGNAL(finished(Tp::PendingOperation *)),
               q, SLOT(__k__onDBusTubeChannelReady(Tp::PendingOperation*)));
}


void AcceptDBusTubeJobPrivate::__k__onDBusTubeChannelReady(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(AcceptDBusTubeJob);

    if (op->isError()) {
        kWarning() << "Unable to make D-Bus tube channel ready -"
            op->errorName() << ":" << op->errorMessage();
        __k__onInvalidated();
        return;
    }

    kDebug() << "DBus tube channel ready!";
    Q_EMIT q->infoMessage(q, i18n("D-Bus tube channel ready."));

    q->connect(channel->acceptTube(),
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onDBusTubeAccepted(Tp::PendingOperation*)));
}


void AcceptDBusTubeJobPrivate::__k__onDBusTubeAccepted(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(AcceptDBusTubeJob);

    if (op->isError()) {
        qWarning() << "Unable to accept dbus tube channel -" <<
            op->errorName() << ":" << op->errorMessage();
        __k__onInvalidated();
        return;
    }

    connection = channel->connection();

    kDebug() << "DBus tube channel accepted and opened!";
    Q_EMIT q->infoMessage(q, i18n("D-Bus tube channel accepted and opened."));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void AcceptDBusTubeJobPrivate::__k__onInvalidated()
{
    Q_Q(AcceptDBusTubeJob);

    kWarning() << "DBus tube invalidated!";
    Q_EMIT q->infoMessage(q, i18n("D-Bus tube invalidated."));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


#include "accept-dbustube-job.moc"
