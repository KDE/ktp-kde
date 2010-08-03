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


#include "accept-file-transfer-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"

#include <QTimer>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/IncomingFileTransferChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingOperation>

class AcceptFileTransferJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(AcceptFileTransferJob)

    public:
        AcceptFileTransferJobPrivate();
        virtual ~AcceptFileTransferJobPrivate();

        Tp::IncomingFileTransferChannelPtr channel;

        QString filename;
        QFile* file;
        qulonglong offset;

        void __k__acceptFileTransfer();
        void __k__onFileTransferChannelReady(Tp::PendingOperation* op);
        void __k__onFileTransferChannelStateChanged(Tp::FileTransferState state, Tp::FileTransferStateChangeReason reason);
        void __k__onFileTransferChannelTransferredBytesChanged(qulonglong count);
        void __k__onAcceptFileFinished(Tp::PendingOperation* op);
        void __k__onInvalidated();

        bool transferCompleted;
};

AcceptFileTransferJob::AcceptFileTransferJob(Tp::ChannelPtr channel,
                                             QString filename,
                                             QObject* parent)
    : TelepathyBaseJob(*new AcceptFileTransferJobPrivate(), parent)
{
    Q_D(AcceptFileTransferJob);

    d->filename = filename;
    d->channel = Tp::IncomingFileTransferChannelPtr::dynamicCast(channel); //TODO check if != 0
}


AcceptFileTransferJob::~AcceptFileTransferJob() {}


void AcceptFileTransferJob::start()
{
    QTimer::singleShot(0, this, SLOT(__k__acceptFileTransfer()));
}


AcceptFileTransferJobPrivate::AcceptFileTransferJobPrivate()
    : file(0)
{
    offset = 0; // TODO?
}


AcceptFileTransferJobPrivate::~AcceptFileTransferJobPrivate()
{
    kDebug();
}


void AcceptFileTransferJobPrivate::__k__acceptFileTransfer()
{
    kDebug();
    Q_Q(AcceptFileTransferJob);

    Tp::Features features = Tp::Features() << Tp::FileTransferChannel::FeatureCore;
    Tp::PendingReady* op = channel->becomeReady(features);

    q->connect(channel.data(), SIGNAL(invalidated(Tp::DBusProxy *, const QString &, const QString &)),
               q, SLOT(__k__onInvalidated()));
    q->connect((QObject*)op, SIGNAL(finished(Tp::PendingOperation *)),
               q, SLOT(__k__onFileTransferChannelReady(Tp::PendingOperation*)));
}


void AcceptFileTransferJobPrivate::__k__onFileTransferChannelReady(Tp::PendingOperation* op)
{
    kDebug();
    Q_Q(AcceptFileTransferJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to make file transfer channel ready") << "-" <<
            op->errorName() << ":" << op->errorMessage();
        __k__onInvalidated();
        return;
    }

    kDebug() << i18n("File transfer channel ready!");
    Q_EMIT q->infoMessage(q, i18n("File transfer channel ready!"));

    q->connect(channel.data(),
               SIGNAL(stateChanged(Tp::FileTransferState, Tp::FileTransferStateChangeReason)),
               SLOT(__k__onFileTransferChannelStateChanged(Tp::FileTransferState, Tp::FileTransferStateChangeReason)));
    q->connect(channel.data(),
               SIGNAL(transferredBytesChanged(qulonglong)),
               SLOT(__k__onFileTransferChannelTransferredBytesChanged(qulonglong)));

    file = new QFile(filename, q->parent());
    kDebug() << i18n("Saving file as") << file->fileName();
    Tp::PendingOperation* acceptFileOperation = channel->acceptFile(offset, file);
    q->connect(acceptFileOperation, SIGNAL(finished(Tp::PendingOperation*)),
               q, SLOT(__k__onAcceptFileFinished(Tp::PendingOperation*)));
    addOperation(acceptFileOperation);
}


void AcceptFileTransferJobPrivate::__k__onFileTransferChannelStateChanged(Tp::FileTransferState state,
                                                                          Tp::FileTransferStateChangeReason stateReason)
{
    kDebug();
    Q_Q(AcceptFileTransferJob);

    kDebug() << i18n("File transfer channel state changed to") << state << i18n("with reason") << stateReason;
    transferCompleted = (state == Tp::FileTransferStateCompleted);
    if (transferCompleted) {
        kDebug() << i18n("Transfer completed, saved at") << file->fileName();
        Q_EMIT q->infoMessage(q, i18n("Transfer completed!"));

        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
    }
}


void AcceptFileTransferJobPrivate::__k__onFileTransferChannelTransferredBytesChanged(qulonglong count)
{
    kDebug();
    Q_Q(AcceptFileTransferJob);

    kDebug().nospace() << i18n("Receiving") << channel->fileName() << "-" << i18n("transferred bytes") << "=" << count << " (" << ((int) (((double) count / channel->size()) * 100)) << "%" << i18n("done") << ")";
    Q_EMIT q->infoMessage(q, i18n("Tranferred bytes"));
}

void AcceptFileTransferJobPrivate::__k__onAcceptFileFinished(Tp::PendingOperation* op)
{
    if (op->isError()) {
        kWarning() << i18n("Unable to accept file transfer") << "-" <<
            op->errorName() << ":" << op->errorMessage();
        __k__onInvalidated();
        return;
    }
}

void AcceptFileTransferJobPrivate::__k__onInvalidated()
{
    Q_Q(AcceptFileTransferJob);

    kWarning() << i18n("File transfer invalidated!");
    Q_EMIT q->infoMessage(q, i18n("File transfer invalidated!"));

    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


#include "accept-file-transfer-job.moc"
