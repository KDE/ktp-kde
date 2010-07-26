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

#include "offer-file-transfer-job.h"
#include "telepathy-base-job_p.h"
#include "telepathy-bridge_p.h"
#include "telepathy-account-proxy.h"

#include <person.h>
#include <personcontact.h>
#include <imaccount.h>

#include <KLocalizedString>
#include <KDebug>

#include <TelepathyQt4/OutgoingFileTransferChannel>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingChannel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Feature>
#include <TelepathyQt4/ConnectionCapabilities>

class OfferFileTransferJobPrivate : public TelepathyBaseJobPrivate
{
    Q_DECLARE_PUBLIC(OfferFileTransferJob)

    public:
        OfferFileTransferJobPrivate(OfferFileTransferJob* parent, OfferFileTransferJob::ProcessingMode m, QString f);
        virtual ~OfferFileTransferJobPrivate();

        OfferFileTransferJob::ProcessingMode mode;

        Tp::OutgoingFileTransferChannelPtr channel;
        Nepomuk::PersonContact contact;
        Nepomuk::Person metacontact;

        QString filename;
        QFile file;

        // Operation Q_PRIVATE_SLOTS
        void __k__offerFileTransferContact();
        void __k__offerFileTransferMetaContact();


        void __k__onFileTransferChannelCreated(Tp::PendingOperation* op);
        void __k__onFileTransferChannelReady(Tp::PendingOperation* op);
        void __k__onFileTransferChannelStateChanged(Tp::FileTransferState state,
                                                    Tp::FileTransferStateChangeReason stateReason);
        void __k__onFileTransferChannelTransferredBytesChanged(qulonglong count);
        void __k__onInvalidated();

        bool findBestContactFromMetaContact();

        bool transferStarted;
};



OfferFileTransferJob::OfferFileTransferJob(const Nepomuk::PersonContact& contact, QString filename, QObject* parent)
    : TelepathyBaseJob(*new OfferFileTransferJobPrivate(this, OfferFileTransferContactMode, filename), parent)
{
    Q_D(OfferFileTransferJob);

    d->contact = contact;
    setCapabilities(KJob::Killable);
}

OfferFileTransferJob::OfferFileTransferJob(const Nepomuk::Person& metacontact, QString filename, QObject* parent)
    : TelepathyBaseJob(*new OfferFileTransferJobPrivate(this, OfferFileTransferMetaContactMode, filename), parent)
{
    Q_D(OfferFileTransferJob);

    d->metacontact = metacontact;
    setCapabilities(KJob::Killable);
}


OfferFileTransferJob::~OfferFileTransferJob()
{
    kDebug();
}


void OfferFileTransferJob::start()
{
    Q_D(OfferFileTransferJob);

    // What are we supposed to do?
    switch (d->mode) {
        case OfferFileTransferContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerFileTransferContact()));
            break;
        case OfferFileTransferMetaContactMode:
            QTimer::singleShot(0, this, SLOT(__k__offerFileTransferMetaContact()));
            break;
        default:
            // Hmm?
            setError(TelepathyBridge::InvalidOperationError);
            setErrorText(i18n("This is an internal error of KDE-Telepathy"));
            QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
            break;
    }
}

bool OfferFileTransferJob::doKill()
{
    kDebug() << i18n("Killing file transfer");
    //TODO suspend the transfer?
    QTimer::singleShot(0, this, SLOT(__k__doEmitResult()));
    return true;
}

OfferFileTransferJobPrivate::OfferFileTransferJobPrivate(OfferFileTransferJob* parent, OfferFileTransferJob::ProcessingMode m, QString f)
    : TelepathyBaseJobPrivate(parent),
      mode(m)
{
    filename = f;
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        kWarning() << "Unable to open file for reading";
        return;
    }

}


OfferFileTransferJobPrivate::~OfferFileTransferJobPrivate() {}


void OfferFileTransferJobPrivate::__k__offerFileTransferContact()
{
    Q_Q(OfferFileTransferJob);

    QHash< TelepathyAccountProxy*, QList< Tp::ContactPtr > > tbd;
    QHash< TelepathyAccountProxy*, QList< Nepomuk::PersonContact > > proxyToContacts;

    QStringList imIDs;

    foreach (const Nepomuk::IMAccount& account, contact.iMAccounts()) {
        imIDs << account.imIDs();
    }

    foreach (TelepathyAccountProxy* proxy, TelepathyBridge::instance()->d_func()->accountProxiesForContact(contact)) {
        tbd[proxy] << proxy->contactsForIdentifiers(imIDs);
        proxyToContacts[proxy] << contact;
    }

    if (tbd.isEmpty()) {
        kWarning() << "No contacts retrieved!";
        // Give it up and emit an error
        q->setError(TelepathyBridge::NoContactsFoundError);
        q->setErrorText(i18n("No existing Telepathy contacts could be mapped to the chosen contact"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

//TODO drdanz check if target contact is online?

    QHash< TelepathyAccountProxy*, QList< Tp::ContactPtr > >::const_iterator i;
    for (i = tbd.constBegin(); i != tbd.constEnd(); ++i) {
        // Can we do it?
        //TODO drdanz: supportsFileTransfers() not yet supported in TpQt4
        //if (i.key()->account()->connection()->capabilities()->supportsFileTransfers()) {
        if (1) {
            // Can target contact accept it?
            //TODO drdanz
            if (1) {
                uint contactHandle = i.value().first()->handle()[0];

                transferStarted = true;
                QFileInfo fileInfo(filename);

                QVariantMap request;
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"), QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_FILE_TRANSFER));
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"), (uint) Tp::HandleTypeContact);
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"), contactHandle);
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_FILE_TRANSFER ".Filename"), fileInfo.fileName());
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_FILE_TRANSFER ".Size"), (qulonglong) fileInfo.size());
                request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_FILE_TRANSFER ".ContentType"), QLatin1String("application/octet-stream"));
                kDebug() << i18n("Request:") << request;

                Tp::PendingChannel* pndChan = i.key()->account()->connection()->createChannel(request);
                q->connect(pndChan,
                           SIGNAL(finished(Tp::PendingOperation*)),
                           SLOT(__k__onFileTransferChannelCreated(Tp::PendingOperation*)));
                return;
            }
        }
    }
    //TODO error file transfer not supported
    q->setError(TelepathyBridge::InvalidOperationError);
    q->setErrorText(i18n("File transfer not supported"));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferFileTransferJobPrivate::__k__offerFileTransferMetaContact()
{
    if(findBestContactFromMetaContact()) {
        __k__offerFileTransferContact();
        return;
    }

    Q_Q(OfferFileTransferJob);
    //TODO error file transfer not supported
    q->setError(TelepathyBridge::InvalidOperationError);
    q->setErrorText(i18n("File transfer not supported"));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


void OfferFileTransferJobPrivate::__k__onFileTransferChannelCreated(Tp::PendingOperation* op)
{
    Q_Q(OfferFileTransferJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to create file transfer channel") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        //TODO Error number
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("Unable to create file transfer channel ready"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        return;
    }

    kDebug() << i18n("File transfer channel created!");
    Q_EMIT q->infoMessage(q, i18n("File transfer channel created!"));

    Tp::PendingChannel* pc = qobject_cast<Tp::PendingChannel*>(op);
    channel = Tp::OutgoingFileTransferChannelPtr::dynamicCast(pc->channel());

    kDebug() << channel.data();

    q->connect(channel.data(),
            SIGNAL(invalidated(Tp::DBusProxy*, const QString&, const QString&)),
            SLOT(__k__onInvalidated()));
    q->connect(channel->becomeReady(Tp::FileTransferChannel::FeatureCore),
               SIGNAL(finished(Tp::PendingOperation*)),
               SLOT(__k__onFileTransferChannelReady(Tp::PendingOperation*)));
}


void OfferFileTransferJobPrivate::__k__onFileTransferChannelReady(Tp::PendingOperation* op)
{
    Q_Q(OfferFileTransferJob);

    if (op->isError()) {
        kWarning() << i18n("Unable to make file transfer channel ready") << "-" <<
            op->errorName() << ": " << op->errorMessage();
        //TODO Error number
        q->setError(TelepathyBridge::InvalidOperationError);
        q->setErrorText(i18n("Unable to make file transfer channel ready"));
        QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
    }

    kDebug() << i18n("File transfer channel ready!");
    Q_EMIT q->infoMessage(q, i18n("File transfer channel ready!"));

    q->connect(channel.data(),
               SIGNAL(stateChanged(Tp::FileTransferState, Tp::FileTransferStateChangeReason)),
               SLOT(__k__onFileTransferChannelStateChanged(Tp::FileTransferState, Tp::FileTransferStateChangeReason)));

    q->connect(channel.data(),
               SIGNAL(transferredBytesChanged(qulonglong)),
               SLOT(__k__onFileTransferChannelTransferredBytesChanged(qulonglong)));

    operations.clear();

    Tp::PendingOperation* provideFileOperation = channel->provideFile(&file);
    Q_UNUSED(provideFileOperation)
//    q->connect(provideFileOperation, SIGNAL(finished(Tp::PendingOperation*)),
//               q, SLOT(__k__onProvideFileFinished(Tp::PendingOperation*)));
//    addOperation(provideFileOperation);
}


void OfferFileTransferJobPrivate::__k__onFileTransferChannelStateChanged(Tp::FileTransferState state, Tp::FileTransferStateChangeReason stateReason)
{
    Q_Q(OfferFileTransferJob);

    kDebug() << i18n("File transfer channel state changed to") << state << i18n("with reason") << stateReason;
    Q_EMIT q->infoMessage(q, i18n("File transfer channel state changed"));

//TODO better handling
/*
case Tp::FileTransferStateChangeReasonNone:
case Tp:: FileTransferStateChangeReasonRequested:
case Tp::FileTransferStateChangeReasonLocalStopped:
case Tp::FileTransferStateChangeReasonRemoteStopped:
case Tp::FileTransferStateChangeReasonLocalError:
case Tp::FileTransferStateChangeReasonRemoteError:
*/
    switch (state)
    {
        case Tp::FileTransferStateNone:
            // This is bad
            kWarning() << i18n("An error occourred.");
            q->setError(TelepathyBridge::InvalidOperationError);
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
        case Tp::FileTransferStateCompleted:
            kDebug() << i18n("Transfer completed");
            Q_EMIT q->infoMessage(q, i18n("Transfer completed"));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult())); //TODO here?
            break;
        case Tp::FileTransferStateCancelled:
            kWarning() << i18n("Transfer was cancelled.");
            q->setError(TelepathyBridge::InvalidOperationError); //TODO
            q->setErrorText(i18n("Transfer was cancelled."));
            QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
            break;
        case Tp::FileTransferStatePending:
        case Tp::FileTransferStateAccepted:
        case Tp::FileTransferStateOpen:
        default:
            break;
    }
}


void OfferFileTransferJobPrivate::__k__onFileTransferChannelTransferredBytesChanged(qulonglong count)
{
    Q_Q(OfferFileTransferJob);

    kDebug() << i18n("Tranferred bytes") << count << " - " << ((int) (((double) count / channel->size()) * 100)) << "%" << i18n("done");
    q->setProcessedAmount(KJob::Bytes, count);
//    Q_EMIT q->infoMessage(q, i18n("Tranferred bytes"));
}


void OfferFileTransferJobPrivate::__k__onInvalidated()
{
    Q_Q(OfferFileTransferJob);

    kWarning() << i18n("File transfer invalidated!");
    Q_EMIT q->infoMessage(q, i18n("File transfer invalidated!"));

    //TODO error file transfer not supported
    q->setError(TelepathyBridge::InvalidOperationError);
    q->setErrorText(i18n("File transfer invalidated"));
    QTimer::singleShot(0, q, SLOT(__k__doEmitResult()));
}


bool OfferFileTransferJobPrivate::findBestContactFromMetaContact()
{
    foreach(Nepomuk::PersonContact foundContact, TelepathyBridge::instance()->d_func()->contactsForMetaContact(metacontact)) {
        // TODO Can we do it?
        if (1) {
            // TODO Can target contact accept it?
            if (1) {
                contact = foundContact;
                return true;
            }
        }
    }
    return false;
}

#include "offer-file-transfer-job.moc"
