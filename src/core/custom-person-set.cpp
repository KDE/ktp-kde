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

#include "custom-person-set.h"

#include <KDebug>

using namespace KTelepathy;

class CustomPersonSet::Private {

public:
    Private()
    { }
};

QSharedPointer<CustomPersonSet> CustomPersonSet::create()
{
    return QSharedPointer<CustomPersonSet>(new CustomPersonSet);
}

CustomPersonSet::CustomPersonSet()
: PersonSet(),
d(new CustomPersonSet::Private)
{
    kDebug();
}

CustomPersonSet::~CustomPersonSet()
{
    kDebug();

    delete d;
}

void CustomPersonSet::addPerson(const PersonPtr &person)
{
    PersonSet::addPerson(person);
}

void CustomPersonSet::removePerson(const PersonPtr &person)
{
    PersonSet::removePerson(person);
}

void CustomPersonSet::removePerson(const QUrl &uri)
{
    PersonSet::removePerson(uri);
}


#include "custom-person-set.moc"

