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

#include <Nepomuk/Thing>

#include <QtGui/QPixmap>

using namespace KTelepathy;

class Person::Private {

public:
    Private()
      : invalidIcon(new KIcon)
    { }

    Nepomuk::Thing pimoPerson;
    ContactSetPtr contacts;

    KIcon *invalidIcon;
    QPixmap invalidPixmap;
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

const QPixmap &Person::avatar() const
{
    // HACK: If there are child contacts, choose the first
    // FIXME: ContactSet population race condition again (and change-notification of course)
    if (d->contacts->contacts().size() >= 1) {
        return d->contacts->contacts().toList().first()->avatar();
    }

    return d->invalidPixmap;
}

QStringList Person::capabilities() const
{
    // HACK: FIXME: TODO: As all the other properties
    if (d->contacts->contacts().size() >= 1) {
        return d->contacts->contacts().toList().first()->capabilities();
    }

    return QStringList();
}

QString Person::displayName() const
{
    // FIXME: Get the display name properly.
    // HACK: If there are child contacts, choose the first
    // FIXME: Race condition populating the contactset here. We should signal when display name is updated somehow.
    if (d->contacts->contacts().size() >= 1) {
        return d->contacts->contacts().toList().first()->displayName();
    }

    return QString();
}

QStringList Person::groups() const
{
    // HACK: FIXME: TODO: As all the other properties
    if (d->contacts->contacts().size() >= 1) {
        return d->contacts->contacts().toList().first()->groups();
    }

    return QStringList();
}

const KIcon &Person::presenceIcon() const
{
    // HACK: FIXME: TODO: As all the other properties
    if (d->contacts->contacts().size() >= 1) {
        return d->contacts->contacts().toList().first()->presenceIcon();
    }

    return *(d->invalidIcon);
}


#include "person.moc"

