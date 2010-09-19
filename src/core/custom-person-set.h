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

#ifndef LIBKTELEPATHY_CUSTOM_PERSON_SET_H
#define LIBKTELEPATHY_CUSTOM_PERSON_SET_H

#include "person-set.h"

#include <kdemacros.h>

#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

class QUrl;

namespace KTelepathy {

/**
 * @class PersonSet
 *
 * This class represents a group of Person objects, allowing bulk actions to be performed on them
 * and for a group of Persons to be stored in a single object. It also emits signals when Persons
 * are added and removed from the PersonSet, allowing subclasses which provide dynamically updated
 * sets of Persons.
 */
class KDE_EXPORT CustomPersonSet : public PersonSet {

    Q_OBJECT

public:
    /**
     * @brief create a new empty person set.
     * @return a shared pointer pointing to the new person set.
     *
     * This method creates a new empty PersonSet and returns a shared pointer to it.
     */
    static QSharedPointer<CustomPersonSet> create();

    /**
     * Destrcutor
     */
    virtual ~CustomPersonSet();

protected:
    /**
     * Protected constructor since library users should create new instances of this class by
     * calling the static member create().
     */
    explicit CustomPersonSet();

    /**
     * @brief add a new person to the set.
     *
     * Add a new person to the Set. The personAdded signal will be emitted if the person added to
     * this set is actually new and not already in the set.
     */
    virtual void addPerson(const PersonPtr &person);

    /**
     * @brief remove a person from the set.
     *
     * Remove a person from the set. The personRemoved signal will be emitted if the person removed
     * from the set is actually in the set to be removed from it.
     */
    virtual void removePerson(const PersonPtr &person);

    /**
     * Overload to remove a person by Nepomuk resource URI only.
     */
    virtual void removePerson(const QUrl &uri);

private:
    Q_DISABLE_COPY(CustomPersonSet);

    class Private;
    Private * const d;

};

typedef QSharedPointer<CustomPersonSet> CustomPersonSetPtr;

}  // namespace KTelepathy


#endif  // Header guard

