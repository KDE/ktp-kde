/*
 * This file is part of libktelepathy
 *
 * Copyright (C) 2010 Collabora Ltd. <info@collabora.co.uk>
 *   @author George Goldberg <george.goldberg@collabora.co.uk>
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

#include "contact.h"

#include "ontologies/nco.h"

#include "ontologies/dataobject.h"
#include "ontologies/imaccount.h"
#include "ontologies/informationelement.h"
#include "ontologies/personcontact.h"

#include <KDebug>
#include <KIcon>
#include <KIconLoader>

#include <Nepomuk/Resource>

#include <QtCore/QByteArray>
#include <QtCore/QPoint>

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include <TelepathyQt4/Constants>

using namespace KTelepathy;

class Contact::Private {

public:
    Private()
    { }

    Nepomuk::PersonContact personContact;
    Nepomuk::IMAccount imAccount;

    KIcon *presenceIcon;
    QPixmap avatar;
};


Contact::Contact(const Nepomuk::Resource &ncoPersonContact, const Nepomuk::Resource &ncoImAccount)
  : Entity(ncoPersonContact),
    d(new Private)
{
    kDebug();

    // FIXME: Check properly if the contact is a suitable Telepathy contact.
    if (ncoPersonContact.hasType(Nepomuk::Vocabulary::NCO::PersonContact())
        && ncoImAccount.hasType(Nepomuk::Vocabulary::NCO::IMAccount()))
    {
        kDebug() << "We have been passed a valid NCO:PersonContact and NCO:IMAccount";
        setValid(true);
        d->personContact = ncoPersonContact;
        d->imAccount = ncoImAccount;
    } else {
        kWarning() << "Person object requires a valid NCO:PersonContact and NCO:IMAccount";
    }

    updatePresenceIconAndAvatar();
}

Contact::Contact()
  : Entity(),
    d(new Private)
{
    kDebug();

    // FIXME: need to initialise the KIcon somehow without Nepomuk, so can't just make the call below.
    // updatePresenceIconAndAvatar();
}

Contact::~Contact()
{
    kDebug();

    delete d;
}

const QPixmap &Contact::avatar() const
{
    // FIXME: Change notification
    return d->avatar;
}

QStringList Contact::capabilities() const
{
    // TODO: Implement me!
    return QStringList();
}

QString Contact::displayName() const
{
    // FIXME: Change notification
    return d->imAccount.imNicknames().first();
}

QStringList Contact::groups() const
{
    // TODO: Implement me!
    return QStringList();
}

const KIcon &Contact::presenceIcon() const
{
    return *(d->presenceIcon);
}

void Contact::updatePresenceIconAndAvatar()
{
    // First, delete the old Icon.
    delete d->presenceIcon;

    // FIXME: Use the correct way to calculate the presence state (not just the PresenceType enum)

    // Now find out the current status.
    QList<qint64> statusTypes = d->imAccount.statusTypes();

    // If no presenceType set, then null KIcon.
    if (statusTypes.size() == 0) {
        d->presenceIcon = new KIcon();
        return;
    }

    // Get the presence type and set the icon appropriately from it.
    QString iconName;

    switch (statusTypes.first()) {
    case Tp::ConnectionPresenceTypeAvailable:
        iconName = QLatin1String("user-online");
        break;
    case Tp::ConnectionPresenceTypeAway:
        iconName = QLatin1String("user-away");
        break;
    case Tp::ConnectionPresenceTypeExtendedAway:
        iconName = QLatin1String("user-away-extended");
        break;
    case Tp::ConnectionPresenceTypeHidden:
        iconName = QLatin1String("user-invisible");
        break;
    case Tp::ConnectionPresenceTypeBusy:
        iconName = QLatin1String("user-busy");
        break;
    default:
        iconName = QLatin1String("user-offline");
        break;
    }

    d->presenceIcon = new KIcon(iconName);

    kDebug() << "Attempt to build the avatar" << d->personContact.resourceUri();
    // Ok, now build the avatar
    d->avatar = QPixmap();
    if (!d->personContact.avatarTokens().isEmpty()) {
        // Load the image then
        if (!d->personContact.photos().isEmpty()) {
            if (!d->personContact.photos().first().interpretedAses().isEmpty()) {
                QByteArray imgdata =
                QByteArray::fromBase64(
                                d->personContact.photos().first().interpretedAses().first().plainTextContents().first().toUtf8());
                QImage image = QImage::fromData(imgdata);
                d->avatar = QPixmap::fromImage(image);
                d->avatar = d->avatar.scaled(32,32);
            }
        }
    }

    if (d->avatar.isNull()) {
        // try to load the action icon
        d->avatar = KIconLoader::global()->loadIcon(QLatin1String("im-user"),
                                                   KIconLoader::NoGroup,
                                                   32,
                                                   KIconLoader::DefaultState,
                                                   QStringList(),
                                                   0,
                                                   true);
    }

    // create a painter to paint the action icon over the key icon
    QPainter painter(&d->avatar);
    // the the emblem icon to size 12
    int overlaySize = 12;
    // try to load the action icon
    const QPixmap iconPixmap = d->presenceIcon->pixmap(overlaySize);
    // if we're able to load the action icon paint it over
    if (!d->avatar.isNull()) {
        QPoint startPoint;
        // bottom right corner
        startPoint = QPoint(32 - overlaySize - 1,
                            32 - overlaySize - 1);
        painter.drawPixmap(startPoint, iconPixmap);
    }
}


#include "contact.moc"

