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

#ifndef LIBKTELEPATHY_OFFER_FILE_TRANSFER_JOB_H
#define LIBKTELEPATHY_OFFER_FILE_TRANSFER_JOB_H

#include <kdemacros.h>

#include "telepathy-base-job.h"

#include <TelepathyQt4/Constants>

class QIODevice;

namespace Nepomuk {
    class PersonContact;
    class Person;
}

namespace Tp {
    class PendingOperation;
}

namespace KTelepathy {

class OfferFileTransferJobPrivate;
class KDE_EXPORT OfferFileTransferJob : public TelepathyBaseJob
{
    Q_OBJECT
    Q_DISABLE_COPY(OfferFileTransferJob)
    Q_DECLARE_PRIVATE(OfferFileTransferJob)

    enum ProcessingMode {
        OfferFileTransferContactMode,
        OfferFileTransferMetaContactMode,
    };

    // Our Q_PRIVATE_SLOTS who perform the real job
    Q_PRIVATE_SLOT(d_func(), void __k__offerFileTransferContact())
    Q_PRIVATE_SLOT(d_func(), void __k__offerFileTransferMetaContact())

    Q_PRIVATE_SLOT(d_func(), void __k__onFileTransferChannelCreated(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onFileTransferChannelReady(Tp::PendingOperation* op))
    Q_PRIVATE_SLOT(d_func(), void __k__onFileTransferChannelStateChanged(Tp::FileTransferState state,
                                                                         Tp::FileTransferStateChangeReason stateReason))
    Q_PRIVATE_SLOT(d_func(), void __k__onFileTransferChannelTransferredBytesChanged(qulonglong count))
    Q_PRIVATE_SLOT(d_func(), void __k__onInvalidated())

    public:
        OfferFileTransferJob(const Nepomuk::PersonContact& contact, QString filename, QObject* parent = 0);
        OfferFileTransferJob(const Nepomuk::Person& metacontact, QString filename, QObject* parent = 0);

        virtual ~OfferFileTransferJob();

        virtual void start();
        virtual bool doKill();
};

} // namespace KTelepathy

#endif // LIBKTELEPATHY_OFFER_FILE_TRANSFER_JOB_H
