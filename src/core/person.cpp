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

#include "person.h"

#include "contact-set.h"

#include "ontologies/nco.h"
#include "ontologies/pimo.h"
#include "ontologies/telepathy.h"

#include <KDebug>
#include <KIcon>
#include <KIconLoader>

#include <Nepomuk/Resource>
#include <Nepomuk/Thing>
#include <Nepomuk/Variant>

#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/NegationTerm>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/ResourceTypeTerm>

using namespace KTelepathy;

class Person::Private {

public:
    Private()
      : mePimoPerson(QUrl::fromEncoded("nepomuk:/myself")),
        presenceIcon(new KIcon)
    { }

    Nepomuk::Thing pimoPerson;
    Nepomuk::Thing mePimoPerson;
    Nepomuk::Query::QueryServiceClient *query;

    QString avatar;
    QSet<QString> capabilities;
    QString displayName;
    QSet<QString> groups;
    KIcon *presenceIcon;
    QString presenceName;
    QString presenceMessage;
    Tp::ConnectionPresenceType presenceType;
};


Person::Person(const Nepomuk::Resource &pimoPerson)
  : ContactSet(),
    Entity(pimoPerson),
    d(new Private)
{
    kDebug();

    // Check the resource we have been passed is a valid PIMO:Person.
    if (pimoPerson.hasType(Nepomuk::Vocabulary::PIMO::Person())) {
        kDebug() << "We have been passed a valid PIMO:Person";
        setValid(true);
        d->pimoPerson = pimoPerson;

        // Setup the contact set.
        d->query = new Nepomuk::Query::QueryServiceClient(this);

        connect(d->query, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
                this, SLOT(onNewEntries(QList<Nepomuk::Query::Result>)));
        connect(d->query, SIGNAL(entriesRemoved(QList<QUrl>)),
                this, SLOT(onEntriesRemoved(QList<QUrl>)));

        connect(this, SIGNAL(contactAdded(KTelepathy::ContactPtr)),
                SLOT(onContactAdded(KTelepathy::ContactPtr)));
        connect(this, SIGNAL(contactRemoved(KTelepathy::ContactPtr)),
                SLOT(onContactRemoved(KTelepathy::ContactPtr)));

        // Get all Telepathy PersonContacts which belong to this PIMO:Person
        {
            using namespace Nepomuk::Query;
            using namespace Nepomuk::Vocabulary;


            // Find grounding occurrences of ME
            ComparisonTerm meGroundingOccurrencesTerm(PIMO::groundingOccurrence(),
                                                      ResourceTerm(d->mePimoPerson));
            meGroundingOccurrencesTerm.setInverted(true);

            // Filter the grounding occurrences to include only those that are PersonContacts.
            AndTerm mePersonContactsTerm(ResourceTypeTerm(NCO::PersonContact()),
                                         meGroundingOccurrencesTerm);

            // Find all the IMAccounts of these person contacts
            ComparisonTerm meImAccountsTerm(NCO::hasIMAccount(),
                                            mePersonContactsTerm);
            meImAccountsTerm.setInverted(true);

            // Get all the IM Accounts that are accessed by this IM account of ours.
            ComparisonTerm isAccessedByTerm(NCO::isAccessedBy(),
                                            ResourceTypeTerm(NCO::IMAccount()));


            // Ensure that the resource accessed through us are IM Accounts.
            AndTerm themImAccountsTerm(ResourceTypeTerm(NCO::IMAccount()),
                                       isAccessedByTerm);

            // Get the PersonContact associated with that IM Account.
            ComparisonTerm themPersonContactsTerm(NCO::hasIMAccount(), themImAccountsTerm);
            themPersonContactsTerm.setVariableName(QLatin1String("account"));

            // Check that the PersonContact is a grounding occurrence of this Person.
            ComparisonTerm themGroundingOccurrencesTerm(PIMO::groundingOccurrence(),
                                                        ResourceTerm(d->pimoPerson));
            themGroundingOccurrencesTerm.setInverted(true);

            // Get the PIMO person which the PersonContact was a grounding occurrence of.
            Query query(AndTerm(ResourceTypeTerm(NCO::PersonContact()),
                                themPersonContactsTerm, themGroundingOccurrencesTerm));

            bool queryResult = d->query->query(query);

            if (!queryResult) {
                kWarning() << "Failed to query the Nepomuk database. QueryServiceClient may not be running";
            }
        }
    } else {
        kWarning() << "Person object requires a valid PIMO:Person";
    }
}

Person::Person()
  : ContactSet(),
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

void Person::onNewEntries(const QList<Nepomuk::Query::Result> &entries)
{
    kDebug();

    foreach (const Nepomuk::Query::Result &entry, entries) {
        kDebug() << entry.resource();
        ContactPtr contact(new Contact(entry.resource(), entry.additionalBinding(QLatin1String("account")).toResource()));
        if (!contact.isNull()) {
            addContact(contact);
        } else {
            kWarning() << "Got a Nepomuk Resource URI that is not a valid NCO:PersonContact";
        }
    }
}

void Person::onEntriesRemoved(const QList<QUrl> &entries)
{
    kDebug();

    foreach (const QUrl &entry, entries) {
        removeContact(entry);
    }
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
    connect(contact.data(),
            SIGNAL(presenceMessageChanged(QString)),
            SLOT(updatePresenceMessage()));
    connect(contact.data(),
            SIGNAL(presenceNameChanged(QString)),
            SLOT(updatePresenceName()));
    connect(contact.data(),
            SIGNAL(presenceTypeChange(Tp::ConnectionPresenceType)),
            SLOT(updatePresenceType()));

    // Update all the properties of the Person.
    updateAvatar();
    updateCapabilities();
    updateDisplayName();
    updateGroups();
    updatePresenceIcon();
    updatePresenceMessage();
    updatePresenceName();
    updatePresenceType();
}

void Person::onContactRemoved(const KTelepathy::ContactPtr &contact)
{
    // Disconnect from the Contact's signals.
    disconnect(contact.data(), SLOT(updateAvatar()));
    disconnect(contact.data(), SLOT(updateCapabilities()));
    disconnect(contact.data(), SLOT(updateDisplayName()));
    disconnect(contact.data(), SLOT(updateGroups()));
    disconnect(contact.data(), SLOT(updatePresenceIcon()));
    disconnect(contact.data(), SLOT(updatePresenceMessage()));
    disconnect(contact.data(), SLOT(updatePresenceName()));
    disconnect(contact.data(), SLOT(updatePresenceType()));

    // Update all the properties of the Person.
    updateAvatar();
    updateCapabilities();
    updateDisplayName();
    updateGroups();
    updatePresenceIcon();
    updatePresenceMessage();
    updatePresenceName();
    updatePresenceType();
}

const QString &Person::avatar() const
{
    return d->avatar;
}

const QSet<QString> &Person::capabilities() const
{
    return d->capabilities;
}

const QString &Person::displayName() const
{
    return d->displayName;
}

const QSet<QString> &Person::groups() const
{
    return d->groups;
}

const KIcon &Person::presenceIcon() const
{
    return *(d->presenceIcon);
}

const QString &Person::presenceMessage() const
{
    return d->presenceMessage;
}

const QString &Person::presenceName() const
{
    return d->presenceName;
}

Tp::ConnectionPresenceType Person::presenceType() const
{
    return d->presenceType;
}

void Person::updateAvatar()
{
    // FIXME: clever way to pick which avatar, rather than the last in the list?
    QString avatar;

    Q_FOREACH (ContactPtr contact, contacts()) {
        if (!contact->avatar().isEmpty()) {
            avatar = contact->avatar();
        }
    }

    // Only signal if an actual change has occurred
    if (avatar != d->avatar) {
        d->avatar = avatar;
        Q_EMIT avatarChanged(d->avatar);
    }
}

void Person::updateCapabilities()
{
    // Person's capabilities are the union of the capabilities of the child contacts.
    QSet<QString> capabilities;

    Q_FOREACH (ContactPtr contact, contacts()) {
        capabilities.unite(contact->capabilities());
    }

    // Only signal if an actual change has occurred.
    if (d->capabilities != capabilities) {
        d->capabilities = capabilities;
        Q_EMIT capabilitiesChanged(d->capabilities);
    }
}

void Person::updateDisplayName()
{
    // FIXME: Choose the most suitable overall displayName some better way.
    QString displayName;

    Q_FOREACH (ContactPtr contact, contacts()) {
        if (!contact->displayName().isEmpty()) {
            displayName = contact->displayName();
        }
    }

    // Only signal if an actual change has occurred.
    if (displayName != d->displayName) {
        d->displayName = displayName;
        Q_EMIT displayNameChanged(d->displayName);
    }
}

void Person::updateGroups()
{
    // Person's groups are the union of all groups of the child contacts.
    QSet<QString> groups;

    Q_FOREACH (ContactPtr contact, contacts()) {
        groups.unite(contact->groups());
    }

    // Only signal if an actual change has occurred.
    if (d->groups != groups) {
        d->groups = groups;
        Q_EMIT groupsChanged(d->groups);
    }
}

void Person::updatePresenceIcon()
{
    // FIXME: Choose the most suitable overall presence some better way.
    KIcon *presenceIcon = 0;

    Q_FOREACH (ContactPtr contact, contacts()) {
        *(presenceIcon) = contact->presenceIcon();
    }

    // Only signal if an actual change has occurred.
    if (presenceIcon != d->presenceIcon) {
        d->presenceIcon = presenceIcon;
        Q_EMIT presenceIconChanged(*(d->presenceIcon));
    }
}

void Person::updatePresenceMessage()
{
    // FIXME: Choose sensibly.
    QString presenceMessage;

    Q_FOREACH (ContactPtr contact, contacts()) {
        presenceMessage = contact->presenceMessage();
    }

    // Only signal if an actual change has occurred
    if (presenceMessage != d->presenceMessage) {
        d->presenceMessage = presenceMessage;
        Q_EMIT presenceMessageChanged(d->presenceMessage);
    }
}

void Person::updatePresenceName()
{
    // FIXME: Choose sensibly.
    QString presenceName;

    Q_FOREACH (ContactPtr contact, contacts()) {
        presenceName = contact->presenceName();
    }

    // Only signal if an actual change has occurred
    if (presenceName != d->presenceName) {
        d->presenceName = presenceName;
        Q_EMIT presenceNameChanged(d->presenceName);
    }
}

void Person::updatePresenceType()
{
    // FIXME: Choose sensibly.
    Tp::ConnectionPresenceType presenceType = Tp::ConnectionPresenceTypeUnknown;

    Q_FOREACH (ContactPtr contact, contacts()) {
        presenceType = contact->presenceType();
    }

    // Only signal if an actual change has occurred
    if (presenceType != d->presenceType) {
        d->presenceType = presenceType;
        Q_EMIT presenceTypeChanged(d->presenceType);
    }
}

uint qHash(const PersonPtr &key) {
    return qHash(key.data());
}


#include "person.moc"

