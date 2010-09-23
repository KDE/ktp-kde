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

#ifndef LIBKTELEPATHY_CONTACT_SET_H
#define LIBKTELEPATHY_CONTACT_SET_H

#include "contact.h"

#include <kdemacros.h>

#include <Nepomuk/Query/Result>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>

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
class KDE_EXPORT ContactSet : public QObject {

    Q_OBJECT

public:
    /**
     * Destrcutor
     */
    virtual ~ContactSet();

    /**
     * @brief Get all the people who are stored in this set.
     * @return a QSet containing PersonPtrs to all the people currently stored in this set.
     *
     * This method provides a QSet with QSharedPointers to all the Person objects currently
     * stored in this PersonSet.
     */
    QSet<ContactPtr> contacts() const;

protected:
    /**
     * Protected constructor since library users should create new instances of this class by
     * calling the static member create().
     */
    explicit ContactSet();

    /**
     * @brief add a new person to the set.
     *
     * Add a new person to the Set. The personAdded signal will be emitted if the person added to
     * this set is actually new and not already in the set.
     */
    void addContact(const ContactPtr &contact);

    /**
     * @brief remove a person from the set.
     *
     * Remove a person from the set. The personRemoved signal will be emitted if the person removed
     * from the set is actually in the set to be removed from it.
     */
    void removeContact(const ContactPtr &contact);

    /**
     * Overload to remove a person by Nepomuk resource URI only.
     */
    void removeContact(const QUrl &uri);

Q_SIGNALS:
    /**
     * @brief emitted when a new person is added to this PersonSet.
     */
    void contactAdded(const KTelepathy::ContactPtr &contact);

    /**
     * @brief emitted when a person is removed from this PersonSet.
     */
    void contactRemoved(const KTelepathy::ContactPtr &contact);

private:
    Q_DISABLE_COPY(ContactSet);

    class Private;
    Private * const d;

    friend class TestBackdoors;

};

typedef QSharedPointer<ContactSet> ContactSetPtr;

}  // namespace KTelepathy


#endif  // Header guard

