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

#include "person-set.h"

#include <KDebug>

#include <QtCore/QSet>

using namespace KTelepathy;

class PersonSet::Private {

public:
    Private()
    { }

    QSet<PersonPtr> people;

};

QSharedPointer<PersonSet> PersonSet::create()
{
    return QSharedPointer<PersonSet>(new PersonSet);
}

PersonSet::PersonSet()
  : QObject(0),
    d(new PersonSet::Private)
{
    kDebug();
}

PersonSet::~PersonSet()
{
    kDebug();

    delete d;
}

void PersonSet::addPerson(const PersonPtr &person)
{
    kDebug();

    // FIXME: Signal should only be emitted if something actually changes
    d->people.insert(person);
    Q_EMIT personAdded(person);
}

void PersonSet::removePerson(const PersonPtr &person)
{
    kDebug();

    // FIXME: Signal should only be emitted if something actually changes
    d->people.remove(person);
    Q_EMIT personRemoved(person);
}

void PersonSet::removePerson(const QUrl &uri)
{
    kDebug();

    Q_FOREACH (const PersonPtr &person, d->people) {
        if (person->resourceUri() == uri) {
            d->people.remove(person);
            Q_EMIT personRemoved(person);
            return;
        }
    }
}

QSet<PersonPtr> PersonSet::people() const
{
    return d->people;
}


#include "person-set.moc"

