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

#ifndef LIBKTELEPATHY_PERSON_H
#define LIBKTELEPATHY_PERSON_H

#include "entity.h"

#include <kdemacros.h>

#include <Nepomuk/Resource>

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>


/**
 * @class Person
 *
 * This class is a subclass of Entity, representing a Person object (basically a wrapper around a
 * PIMO:Person in the Nepomuk store).
 */
class KDE_EXPORT Person : public Entity {

    Q_OBJECT

public:
    virtual ~Person();

private:
    // Constrcutor is private because only PeopleManager should directly create Person objects
    explicit Person(const Nepomuk::Resource &pimoPerson);
    friend class PeopleManager;

    class Private;
    Private * const d;

};

typedef QSharedPointer<Person> PersonPtr;


#endif  // Header guard

