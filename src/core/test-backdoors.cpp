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

#include "test-backdoors.h"

using namespace KTelepathy;

PersonSetPtr TestBackdoors::personSetCreate()
{
    return QSharedPointer<PersonSet>(new PersonSet());
}

void TestBackdoors::personSetAddPerson(PersonSetPtr personSet, PersonPtr person)
{
    Q_ASSERT(personSet);

    personSet->addPerson(person);
}

void TestBackdoors::personSetRemovePerson(PersonSetPtr personSet, PersonPtr person)
{
    Q_ASSERT(personSet);

    personSet->removePerson(person);
}

void TestBackdoors::personSetRemovePerson(PersonSetPtr personSet, const QUrl &url)
{
    Q_ASSERT(personSet);

    personSet->removePerson(url);
}

EntityPtr TestBackdoors::entityConstruct(const Nepomuk::Resource &resource)
{
    return EntityPtr(new Entity(resource));
}

EntityPtr TestBackdoors::entityConstruct()
{
    return EntityPtr(new Entity());
}

void TestBackdoors::entitySetValid(EntityPtr entity, bool valid)
{
    Q_ASSERT(entity);

    entity->setValid(valid);
}


