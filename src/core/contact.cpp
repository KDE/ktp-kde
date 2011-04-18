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
#include "ontologies/telepathy.h"

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

#include <Soprano/Node>
#include <Soprano/Statement>

#include <TelepathyQt4/Constants>

using namespace KTelepathy;

class Contact::Private {

public:
    Private()
      : presenceIcon(0)
    { }

    Nepomuk::PersonContact personContact;
    Nepomuk::IMAccount imAccount;

    KIcon *presenceIcon;
    QPixmap avatar;
};


Contact::Contact(const Nepomuk::Resource &ncoPersonContact, const Nepomuk::Resource &ncoImAccount)
  : QObject(0),
    Entity(ncoPersonContact),
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

        // Watch for changes on both the IMAccount and the PersonContact.
        NepomukSignalWatcher *sw = NepomukSignalWatcher::instance();
        sw->registerCallbackOnSubject(d->personContact, this);
        sw->registerCallbackOnSubject(d->imAccount, this);
    } else {
        kWarning() << "Person object requires a valid NCO:PersonContact and NCO:IMAccount";
    }

    updatePresenceIcon();
    updateAvatar();
    updateDisplayName();
}

Contact::Contact()
  : QObject(0),
    Entity(),
    d(new Private)
{
    kDebug();

    // FIXME: need to initialise the KIcon somehow without Nepomuk, so can't just make the call below.
    // updatePresenceIconAndAvatar();
}

Contact::~Contact()
{
    kDebug();

    // Unregister nepomuk watcher callbacks OR THERE WILL BE A CRASH!
    NepomukSignalWatcher *sw = NepomukSignalWatcher::instance();
    sw->unregisterCallbackOnSubject(d->personContact, this);
    sw->unregisterCallbackOnSubject(d->imAccount, this);

    delete d;
}

// FIXME: Use a flag, not a bool
const QPixmap &Contact::avatar() const
{
    // FIXME: Change notification
    return d->avatar;
}

QSet<QString> Contact::capabilities() const
{
    // TODO: Implement me!
    return QSet<QString>();
}

QString Contact::displayName() const
{
    // FIXME: Change notification
    if (d->imAccount.imNicknames().isEmpty()) {
        return QString();
    }

    return d->imAccount.imNicknames().first();
}

QSet<QString> Contact::groups() const
{
    // TODO: Implement me!
    return QSet<QString>();
}

const KIcon &Contact::presenceIcon() const
{
    return *(d->presenceIcon);
}

QString Contact::presenceMessage() const
{
    return d->imAccount.imStatusMessage();
}

QString Contact::presenceName() const
{
    return d->imAccount.imStatus();
}


void Contact::onStatementAdded(const Soprano::Statement& statement)
{
    kDebug() << "Statement added.";
    Soprano::Node subject = statement.subject();
    Soprano::Node predicate = statement.predicate();
    Soprano::Node object = statement.object();

    // Deal with the statement if it is added to the personContact.
    if (subject.uri() == d->personContact.resourceUri()) {

        // TODO: Implement me!

        kWarning() << "Unrecognised statement added to PersonContact.";
        return;
    }

    // Deal with the statement if it is added to the imAccount
    if (subject.uri() == d->imAccount.resourceUri()) {
        if (predicate.uri() == Nepomuk::Vocabulary::NCO::imNickname()) {
            updateDisplayName();
            return;
        }

        if ((predicate.uri() == Nepomuk::Vocabulary::NCO::imStatus())
            || (predicate.uri() == Nepomuk::Vocabulary::Telepathy::statusType()))
        {
            // FIXME: separate out presence icon and avatar update methods.
            updatePresenceIcon();
            updateAvatar();
            return;
        }

        if (predicate.uri() == Nepomuk::Vocabulary::NCO::photo()) {
            updateAvatar();
            return;
        }

        kWarning() << "Unrecognised statement added to IMAccount.";
        return;
    }

    kWarning() << "Unrecognised statement added.";
}

void Contact::onStatementRemoved(const Soprano::Statement& statement)
{
    kDebug() << "Statement removed.";
}

void Contact::updateAvatar()
{
    // FIXME: Only update the avatar if it has actually changed
    kDebug() << "Attempt to build the avatar" << d->personContact.resourceUri();
    // Ok, now build the avatar
    d->avatar = QPixmap();
/*    if (!d->personContact.avatarTokens().isEmpty()) {
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

    Q_EMIT avatarChanged(d->avatar);
*/
}

void Contact::updateDisplayName()
{
    // TODO: Implement me!
}

void Contact::updatePresenceIcon()
{
    // FIXME: Only recreate the icon if it has actually changed.

    // First, delete the old Icon.
    delete d->presenceIcon;

    // FIXME: Use the correct way to calculate the presence state (not just the PresenceType enum)

    // Get the presence type and set the icon appropriately from it.
    QString iconName;

    switch (d->imAccount.statusType()) {
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

    Q_EMIT presenceIconChanged(*d->presenceIcon);
}

uint qHash(const ContactPtr &key) {
    return qHash(key.data());
}


#include "contact.moc"

