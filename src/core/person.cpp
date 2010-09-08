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

#include <Nepomuk/Thing>

using namespace KTelepathy;

class Person::Private {

public:
    Private()
    { }

    Nepomuk::Thing pimoPerson;
    ContactSetPtr contacts;

};


Person::Person(const Nepomuk::Resource &pimoPerson)
  : Entity(pimoPerson),
    d(new Private)
{
    kDebug();

    // Check the resource we have been passed is a valid PIMO:Person.
    if (pimoPerson.hasType(Nepomuk::Vocabulary::PIMO::Person())) {
        // Wrong type. Invalid.
        kDebug() << "We have been passed a valid PIMO:Person";
        setValid(true);
        d->pimoPerson = pimoPerson;
        d->contacts = QSharedPointer<ContactSet>(new ContactSet(d->pimoPerson));
    } else {
        kWarning() << "Person object requires a valid PIMO:Person";
    }
}

Person::Person()
  : Entity(),
    d(new Private)
{
    kDebug();
}

Person::~Person()
{
    kDebug();

    delete d;
}

ContactSetPtr Person::contacts() const
{
    return d->contacts;
}


#include "person.moc"

