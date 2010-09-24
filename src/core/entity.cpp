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

#include "entity.h"

#include <KDebug>

using namespace KTelepathy;

class Entity::Private {

public:
    Private()
      : valid(false)
    { }

    bool valid;
    Nepomuk::Resource resource;
    QUrl resourceUri;
};

Entity::Entity(const Nepomuk::Resource &resource)
  : d(new Entity::Private)
{
    kDebug();

    d->resource = resource;
    d->resourceUri = resource.resourceUri();
}

Entity::Entity()
  : d(new Entity::Private)
{
    kDebug();
}

Entity::~Entity()
{
    kDebug();

    delete d;
}

bool Entity::isValid() const
{
    return d->valid;
}

Nepomuk::Resource Entity::resource() const
{
    // If the entity is invalid, return an invalid Nepomuk Resource
    if (!d->valid) {
        return Nepomuk::Resource();
    }

    return d->resource;
}

QUrl Entity::resourceUri() const
{
    return d->resourceUri;
}

void Entity::setValid(bool valid)
{
    d->valid = valid;
}

