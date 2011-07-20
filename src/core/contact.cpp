/*
 * This file is part of libktelepathy
 *
 * Copyright (C) 2010-2011 Collabora Ltd. <info@collabora.co.uk>
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

#include "ontologies/contactgroup.h"
#include "ontologies/dataobject.h"
#include "ontologies/imaccount.h"
#include "ontologies/imcapability.h"
#include "ontologies/personcontact.h"

#include <KDebug>
#include <KIcon>
#include <KIconLoader>

#include <Nepomuk/Resource>

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

    QString avatar;
    QSet<QString> capabilities;
    QString displayName;
    QSet<QString> groups;
    KIcon *presenceIcon;
    QString presenceMessage;
    QString presenceName;
    Tp::ConnectionPresenceType presenceType;
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

    updateAvatar();
    updateCapabilities();
    updateDisplayName();
    updateGroups();
    updatePresenceIcon();
    updatePresenceMessage();
    updatePresenceName();
    updatePresenceType();
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

const QString &Contact::avatar() const
{
    return d->avatar;
}

const QSet<QString> &Contact::capabilities() const
{
    return d->capabilities;
}

const QString &Contact::displayName() const
{
    return d->displayName;
}

const QSet<QString> &Contact::groups() const
{
    return d->groups;
}

const KIcon &Contact::presenceIcon() const
{
    return *(d->presenceIcon);
}

const QString &Contact::presenceMessage() const
{
    return d->presenceMessage;
}

const QString &Contact::presenceName() const
{
    return d->presenceName;
}

Tp::ConnectionPresenceType Contact::presenceType() const
{
    return d->presenceType;
}

void Contact::onStatementAdded(const Soprano::Statement& statement)
{
    kDebug() << "Statement added.";
    Soprano::Node subject = statement.subject();
    Soprano::Node predicate = statement.predicate();
    Soprano::Node object = statement.object();

    // Deal with the statement if it is added to the personContact.
    if (subject.uri() == d->personContact.resourceUri()) {

        // Changes affecting the contact groups
        if (predicate.uri() == Nepomuk::Vocabulary::NCO::belongsToGroup()) {
            updateGroups();
        }

        kWarning() << "Unrecognised statement added to PersonContact.";
        return;
    }

    // Deal with the statement if it is added to the imAccount
    if (subject.uri() == d->imAccount.resourceUri()) {

        // Changes affecting the display name
        if (predicate.uri() == Nepomuk::Vocabulary::NCO::imNickname()) {
            updateDisplayName();
            return;
        }

        // Changes affecting the presenceName, presenceType and presenceIcon
        if ((predicate.uri() == Nepomuk::Vocabulary::NCO::imStatus())
            || (predicate.uri() == Nepomuk::Vocabulary::Telepathy::statusType()))
        {
            updatePresenceIcon();
            updatePresenceName();
            updatePresenceType();
            return;
        }

        // Changes affecting the capabilities
        if (predicate.uri() == Nepomuk::Vocabulary::NCO::hasIMCapability()) {
            updateCapabilities();
            return;
        }

        // Changes affecting the presence Message
        if (predicate.uri() == Nepomuk::Vocabulary::NCO::imStatusMessage()) {
            updatePresenceMessage();
            return;
        }

        if (predicate.uri() == Nepomuk::Vocabulary::Telepathy::avatar()) {
            updateAvatar();
            return;
        }

        kWarning() << "Unrecognised statement added to IMAccount:" << predicate.uri();
        return;
    }

    kWarning() << "Unrecognised statement added.";
}

void Contact::onStatementRemoved(const Soprano::Statement& statement)
{
    kDebug() << "Statement removed.";

    // Assume that the same properties are affected whether the statement is being added or removed.
    onStatementAdded(statement);
}

void Contact::updateAvatar()
{
    QString avatar;

    avatar = d->imAccount.avatar().resourceUri().toString();

    // Only signal if an actual change has occurred
    if (avatar != d->avatar) {
        d->avatar = avatar;
        Q_EMIT avatarChanged(d->avatar);
    }
}

void Contact::updateCapabilities()
{
    QList<Nepomuk::IMCapability> caps = d->imAccount.iMCapabilitys();

    QSet<QString> capabilities;

    Q_FOREACH (const Nepomuk::IMCapability &cap, caps) {
        if (cap == Nepomuk::Vocabulary::NCO::imCapabilityText()) {
            capabilities.insert(QLatin1String("text"));
        } else if (cap == Nepomuk::Vocabulary::NCO::imCapabilityAudio()) {
            capabilities.insert(QLatin1String("audio"));
        } else if (cap == Nepomuk::Vocabulary::NCO::imCapabilityVideo()) {
            capabilities.insert(QLatin1String("video"));
        } else {
            kWarning() << "Unrecognised capability: " << cap.resourceUri();
        }
    }

    // Only signal if an actual change has occurred
    if (capabilities != d->capabilities) {
        d->capabilities = capabilities;
        Q_EMIT capabilitiesChanged(d->capabilities);
    }
}

void Contact::updateDisplayName()
{
    QString displayName;

    if (d->imAccount.imNicknames().size() > 0) {
        displayName = d->imAccount.imNicknames().first();
    } else {
        displayName.clear();
    }

    // Only signal if an actual change has occurred
    if (displayName != d->displayName) {
        d->displayName = displayName;
        Q_EMIT displayNameChanged(d->displayName);
    }
}

void Contact::updateGroups()
{
     QList<Nepomuk::ContactGroup> contactGroups = d->personContact.belongsToGroups();

     QSet<QString> groups;

     Q_FOREACH (const Nepomuk::ContactGroup &group, contactGroups) {
         groups.insert(group.contactGroupName());
     }

    // Only signal if an actual change has occurred
    if (groups != d->groups) {
        d->groups = groups;
        Q_EMIT groupsChanged(d->groups);
    }
}

void Contact::updatePresenceIcon()
{
    // FIXME: We need the code to figure out the Icon to be centrally implemented just once in the
    // lib so that all KDE/Telepathy applications show the presence in a consistent way.

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

    // Only signal if an actual change has occurred
    if (!(d->presenceIcon && d->presenceIcon->name() == iconName)) {
        delete d->presenceIcon;
        d->presenceIcon = new KIcon(iconName);

        Q_EMIT presenceIconChanged(*d->presenceIcon);
    }
}

void Contact::updatePresenceMessage()
{
    QString presenceMessage;

    presenceMessage = d->imAccount.imStatusMessage();

    // Only signal if an actual change has occurred
    if (presenceMessage != d->presenceMessage) {
        d->presenceMessage = presenceMessage;
        Q_EMIT presenceMessageChanged(d->presenceMessage);
    }
}

void Contact::updatePresenceName()
{
    QString presenceName;

    presenceName = d->imAccount.imStatus();

    // Only signal if an actual change has occurred
    if (presenceName != d->presenceName) {
        d->presenceName = presenceName;
        Q_EMIT presenceNameChanged(d->presenceName);
    }
}

void Contact::updatePresenceType()
{
    Tp::ConnectionPresenceType presenceType;

    presenceType = static_cast<Tp::ConnectionPresenceType>(d->imAccount.statusType());

    // Only signal if an actual change has occurred
    if (presenceType != d->presenceType) {
        d->presenceType = presenceType;
        Q_EMIT presenceTypeChanged(d->presenceType);
    }
}


uint qHash(const ContactPtr &key) {
    return qHash(key.data());
}


#include "contact.moc"

