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

#include "contact-set.h"

#include "ontologies/nco.h"
#include "ontologies/pimo.h"
#include "ontologies/telepathy.h"

#include <KDebug>

#include <QtCore/QSet>

using namespace KTelepathy;

class ContactSet::Private {

public:
    Private()
    { }

    QSet<ContactPtr> contacts;
};

ContactSet::ContactSet()
  : QObject(0),
    d(new ContactSet::Private)
{
    kDebug();
}

ContactSet::~ContactSet()
{
    kDebug();

    delete d;
}

void ContactSet::addContact(const ContactPtr &contact)
{
    kDebug();

    // Signal should only be emitted if something actually changes
    if (!d->contacts.contains(contact)) {
        d->contacts.insert(contact);
        Q_EMIT contactAdded(contact);
    }
}

void ContactSet::removeContact(const ContactPtr &contact)
{
    kDebug();

    // Signal should only be emitted if something actually changes
    if (d->contacts.contains(contact)) {
        d->contacts.remove(contact);
        Q_EMIT contactRemoved(contact);
    }
}

void ContactSet::removeContact(const QUrl &uri)
{
    kDebug();

    Q_FOREACH (const ContactPtr &contact, d->contacts) {
        if (contact->resourceUri() == uri) {
            d->contacts.remove(contact);
            Q_EMIT contactRemoved(contact);
            return;
        }
    }
}

QSet<ContactPtr> ContactSet::contacts() const
{
    return d->contacts;
}


#include "contact-set.moc"

