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

#include "ontologies/personcontact.h"

#include <KDebug>

#include <Nepomuk/Resource>

using namespace KTelepathy;

class Contact::Private {

public:
    Private()
    { }

    Nepomuk::PersonContact personContact;
};


Contact::Contact(const Nepomuk::Resource &ncoPersonContact)
  : Entity(ncoPersonContact),
    d(new Private)
{
    kDebug();

    // FIXME: Check properly if the contact is a suitable Telepathy contact.
    if (ncoPersonContact.hasType(Nepomuk::Vocabulary::NCO::PersonContact())) {
        kDebug() << "We have been passed a valid NCO:PersonContact";
        setValid(true);
        d->personContact = ncoPersonContact;
    } else {
        kWarning() << "Person object requires a valid NCO:PersonContact";
    }
}

Contact::Contact()
  : Entity(),
    d(new Private)
{
    kDebug();
}

Contact::~Contact()
{
    kDebug();

    delete d;
}


#include "contact.moc"

