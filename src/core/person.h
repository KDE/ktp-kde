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

#include "contact-set.h"

#include <kdemacros.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace Nepomuk {
    class Resource;
}

class KIcon;
class QPixmap;

namespace KTelepathy {

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

    /**
     * Return a shared-pointer to the ContactSet containing all the contacts that are linked
     * to this Person.
     *
     * If the Person is invalid, the ContactSet returned may be null.
     */
    ContactSetPtr contacts() const;

    /**
     * Returns the avatar of this person
     */
    const QPixmap &avatar() const;

    /**
     * Returns the capabilities of this person
     */
    QStringList capabilities() const;

    /**
     * Returns the display name of the person
     */
    QString displayName() const;

    /**
     * Returns the groups to which this person belongs
     */
    QStringList groups() const;

    /**
     * Returns the presence Icon for this person
     */
    const KIcon &presenceIcon() const;

protected:
    /**
     * Constrcutor is protected because only PeopleManager should directly create Person objects.
     */
    explicit Person(const Nepomuk::Resource &pimoPerson);

    /**
     * Construct an invalid Person object
     */
    Person();

private:
    Q_DISABLE_COPY(Person);

    friend class PeopleManager;

    class Private;
    Private * const d;

};

typedef QSharedPointer<Person> PersonPtr;

}  // namespace KTelepathy

Q_DECLARE_METATYPE(QSharedPointer<KTelepathy::Person>);


#endif  // Header guard

