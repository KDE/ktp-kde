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

#ifndef LIBKTELEPATHY_ACCEPT_DBUSTUBE_JOB_H
#define LIBKTELEPATHY_ACCEPT_DBUSTUBE_JOB_H

#include <kdemacros.h>

#include "telepathy-base-job.h"

#include <QVariantMap>
#include <TelepathyQt4/Channel>

namespace Tp {
    class IncomingDBusTubeChannel;
}

class QDBusConnection;

class AcceptDBusTubeJobPrivate;
class KDE_EXPORT AcceptDBusTubeJob : public TelepathyBaseJob
{
    Q_OBJECT
    Q_DISABLE_COPY(AcceptDBusTubeJob)
    Q_DECLARE_PRIVATE(AcceptDBusTubeJob)

    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__acceptDBusTube())
    Q_PRIVATE_SLOT(d_func(), void __k__onDBusTubeChannelReady(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onDBusTubeAccepted(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onInvalidated())

    public:
//TODO?        AcceptDBusTubeJob(const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);
        AcceptDBusTubeJob(Tp::ChannelPtr channel, const QVariantMap& parameters = QVariantMap(), QObject* parent = 0);

        virtual ~AcceptDBusTubeJob();

        virtual void start();

        QDBusConnection dbusConnection();

        Tp::IncomingDBusTubeChannelPtr incomingDBusTubeChannel();
};

#endif // LIBKTELEPATHY_ACCEPT_DBUSTUBE_JOB_H
