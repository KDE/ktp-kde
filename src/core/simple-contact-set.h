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

#ifndef LIBKTELEPATHY_SIMPLE_CONTACT_SET_H
#define LIBKTELEPATHY_SIMPLE_CONTACT_SET_H

#include "contact-set.h"

#include <kdemacros.h>

#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

class QUrl;

namespace KTelepathy {

/**
    * @class SimpleContactSet
    *
    */
class KDE_EXPORT SimpleContactSet : public ContactSet {

    Q_OBJECT

public:
    /**
     */
    static QSharedPointer<SimpleContactSet> create();

    /**
     * Destrcutor
     */
    virtual ~SimpleContactSet();

    /**
     */
    virtual void addContact(const ContactPtr &contact);

    /**
     */
    virtual void removePerson(const ContactPtr &contact);

    /**
     * Overload to remove a contact by Nepomuk resource URI only.
     */
    virtual void removeContact(const QUrl &uri);

protected:
    /**
     * Protected constructor since library users should create new instances of this class by
     * calling the static member create().
     */
    explicit SimpleContactSet();

private:
    Q_DISABLE_COPY(SimpleContactSet);

    class Private;
    Private * const d;

};

typedef QSharedPointer<SimpleContactSet> SimpleContactSetPtr;

}  // namespace KTelepathy


#endif  // Header guard

