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

#ifndef LIBKTELEPATHY_REQUESTP2PDBUSTUBEJOB_H
#define LIBKTELEPATHY_REQUESTP2PDBUSTUBEJOB_H


#include <abstractrequestchanneljob.h>
#include <kdemacros.h>

class RequestP2PDBusTubeJobPrivate;
class KDE_EXPORT RequestP2PDBusTubeJob : public AbstractRequestChannelJob
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestP2PDBusTubeJob)
    Q_DECLARE_PRIVATE(RequestP2PDBusTubeJob)

public:
    RequestP2PDBusTubeJob(RequestP2PDBusTubeJobPrivate &dd, QObject *parent = 0);
    virtual ~RequestP2PDBusTubeJob();

    virtual Tp::PendingChannelRequest* createChannel();
    virtual bool canCreateChannel();
};


namespace Nepomuk {
    class PersonContact;
    class Person;
}


/*
KDE_EXPORT RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::PersonContact& contact,
                                                     const QString& serviceName,
                                                     const QString& preferredHandler = QString(),
                                                     const RequestChannelJob::RequestFlags requestchannelflags = RequestChannelJob::RequestModeEnsure,
                                                     QObject* parent = 0);
KDE_EXPORT RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::Person& metacontact,
                                                     const QString& serviceName,
                                                     const QString& preferredHandler = QString(),
                                                     const RequestChannelJob::RequestFlags requestchannelflags = RequestChannelJob::RequestModeEnsure,
                                                     QObject* parent = 0);
*/
KDE_EXPORT RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::PersonContact& contact,
                                                     const QString& serviceName,
                                                     const QString& preferredHandler,
                                                     QObject* parent);
KDE_EXPORT RequestP2PDBusTubeJob* requestP2PDBusTube(const Nepomuk::Person& metacontact,
                                                     const QString& serviceName,
                                                     const QString& preferredHandler,
                                                     QObject* parent);
#endif // LIBKTELEPATHY_REQUESTP2PDBUSTUBEJOB_H
