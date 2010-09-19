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

#include "person.h"

#include "contact-set.h"

#include "ontologies/pimo.h"

#include <KDebug>
#include <KIcon>
#include <KIconLoader>

#include <Nepomuk/Thing>

#include <QtCore/QSet>

#include <QtGui/QPainter>
#include <QtGui/QPixmap>

using namespace KTelepathy;

class Person::Private {

public:
    Private()
      : presenceIcon(new KIcon)
    { }

    Nepomuk::Thing pimoPerson;
    ContactSetPtr contacts;

    QPixmap avatar;
    QPixmap avatarWithOverlay;
    QSet<QString> capabilities;
    QString displayName;
    QSet<QString> groups;
    KIcon *presenceIcon;
};


Person::Person(const Nepomuk::Resource &pimoPerson)
  : QObject(0),
    Entity(pimoPerson),
    d(new Private)
{
    kDebug();

    // Check the resource we have been passed is a valid PIMO:Person.
    if (pimoPerson.hasType(Nepomuk::Vocabulary::PIMO::Person())) {
        kDebug() << "We have been passed a valid PIMO:Person";
        setValid(true);
        d->pimoPerson = pimoPerson;
        d->contacts = QSharedPointer<ContactSet>(new ContactSet(d->pimoPerson));
        connect(d->contacts.data(),
                SIGNAL(contactAdded(const KTelepathy::ContactPtr &)),
                SLOT(onContactAdded(const KTelepathy::ContactPtr &)));
        connect(d->contacts.data(),
                SIGNAL(contactRemoved(const KTelepathy::ContactPtr &)),
                SLOT(onContactRemoved(const KTelepathy::ContactPtr &)));
    } else {
        kWarning() << "Person object requires a valid PIMO:Person";
    }
}

Person::Person()
  : QObject(0),
    Entity(),
    d(new Private)
{
    kDebug();
}

Person::~Person()
{
    kDebug();

    delete d;
}

void Person::onContactAdded(const KTelepathy::ContactPtr &contact)
{
    // Connect to the signals of the new contact.
    connect(contact.data(),
            SIGNAL(avatarChanged(QPixmap)),
            SLOT(updateAvatar()));
    connect(contact.data(),
            SIGNAL(capabilitiesChanged(QSet<QString>)),
            SLOT(updateCapabilities()));
    connect(contact.data(),
            SIGNAL(displayNameChanged(QString)),
            SLOT(updateDisplayName()));
    connect(contact.data(),
            SIGNAL(groupsChanged(QSet<QString>)),
            SLOT(updateGroups()));
    connect(contact.data(),
            SIGNAL(presenceIconChanged(KIcon)),
            SLOT(updatePresenceIcon()));

    // Update all the properties of the Person.
    updateAvatar();
    updateCapabilities();
    updateDisplayName();
    updateGroups();
    updatePresenceIcon();
}

void Person::onContactRemoved(const KTelepathy::ContactPtr &contact)
{
    // Disconnect from the Contact's signals.
    disconnect(contact.data(), SLOT(updateAvatar()));
    disconnect(contact.data(), SLOT(updateCapabilities()));
    disconnect(contact.data(), SLOT(updateDisplayName()));
    disconnect(contact.data(), SLOT(updateGroups()));
    disconnect(contact.data(), SLOT(updatePresenceIcon()));

    // Update all the properties of the Person.
    updateAvatar();
    updateCapabilities();
    updateDisplayName();
    updateGroups();
    updatePresenceIcon();
}

ContactSetPtr Person::contacts() const
{
    return d->contacts;
}

// FIXME: Use flags instead of a bool for withOverlay?
const QPixmap &Person::avatar(bool withOverlay) const
{
    return withOverlay ? d->avatarWithOverlay : d->avatar;
}

QSet<QString> Person::capabilities() const
{
    return d->capabilities;
}

QString Person::displayName() const
{
    return d->displayName;
}

QSet<QString> Person::groups() const
{
    return d->groups;
}

const KIcon &Person::presenceIcon() const
{
    return *(d->presenceIcon);
}

void Person::updateAvatar()
{
    d->avatar = QPixmap();

    // FIXME: clever way to pick which avatar, rather than the last in the list?
    Q_FOREACH (ContactPtr contact, d->contacts->contacts()) {
        if (!contact->avatar().isNull()) {
            d->avatar = contact->avatar();
        }
    }

    // If the avatar is null on all child contacts, put a generic avatar image for the Person.
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

    Q_EMIT avatarChanged(d->avatar);

    // Since the avatar has changed, we must update the avatar with overlay too.
    updateAvatarWithOverlay();
}

void Person::updateAvatarWithOverlay()
{
    // Copy the avatar pixmap ready for updating the overlayed version.
    d->avatarWithOverlay = d->avatar;

    // create a painter to paint the action icon over the key icon
    QPainter painter(&d->avatarWithOverlay);
    // the the emblem icon to size 12
    int overlaySize = 12;
    // try to load the action icon
    const QPixmap iconPixmap = d->presenceIcon->pixmap(overlaySize);
    // if we're able to load the action icon paint it over
    if (!d->avatarWithOverlay.isNull()) {
        QPoint startPoint;
        // bottom right corner
        startPoint = QPoint(32 - overlaySize - 1,
                            32 - overlaySize - 1);
        painter.drawPixmap(startPoint, iconPixmap);
    }

    Q_EMIT avatarWithOverlayChanged(d->avatarWithOverlay);
}

void Person::updateCapabilities()
{
    // Person's capabilities are the union of the capabilities of the child contacts.
    QSet<QString> capabilities;
    Q_FOREACH (ContactPtr contact, d->contacts->contacts()) {
        capabilities.unite(contact->capabilities());
    }

    // Only signal the change if something actually changed.
    if (d->capabilities != capabilities) {
        d->capabilities = capabilities;
        Q_EMIT capabilitiesChanged(d->capabilities);
    }
}

void Person::updateDisplayName()
{
    // FIXME: Choose the most suitable overall displayName some better way.
    Q_FOREACH (ContactPtr contact, d->contacts->contacts()) {
        d->displayName = contact->displayName();
    }

    // FIXME: Only emit this signal if the display name actually changed.
    Q_EMIT displayNameChanged(d->displayName);
}

void Person::updateGroups()
{
    // Person's groups are the union of all groups of the child contacts.
    QSet<QString> groups;
    Q_FOREACH (ContactPtr contact, d->contacts->contacts()) {
        groups.unite(contact->groups());
    }

    // Only signal the change if something has actually changed.
    if (d->groups != groups) {
        d->groups = groups;
        Q_EMIT groupsChanged(d->groups);
    }
}

void Person::updatePresenceIcon()
{
    // FIXME: Choose the most suitable overall presence some better way.
    Q_FOREACH (ContactPtr contact, d->contacts->contacts()) {
        *(d->presenceIcon) = contact->presenceIcon();
    }

    // FIXME: Only emit this signal if the presenceIcon has actually changed.
    Q_EMIT presenceIconChanged(*(d->presenceIcon));

    // Since the presence icon has changed, we must now update the avatar with the presence
    // icon overlayed on it.
    updateAvatarWithOverlay();
}


#include "person.moc"

