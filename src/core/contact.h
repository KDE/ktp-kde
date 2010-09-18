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

#ifndef LIBKTELEPATHY_CONTACT_H
#define LIBKTELEPATHY_CONTACT_H

#include "entity.h"

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
 * @class Contact
 *
 * This class is a subclass of Entity, representing a Contact object (basically a wrapper around a
 * NCO:PersonContact in the Nepomuk store). All contact objects belong to a Person object, and
 * cannot exist on their own. Contact objects should only be used where strictly necessary, since
 * most desired functions can be acheived using Person objects alone. The main scenario where
 * Contact objects should be used is when the application wishes to specify the exact
 * local account/buddy combination that should be used when opening a channel, rather than allowing
 * libktelepathy to automagically use the most suitable one.
 */
class KDE_EXPORT Contact : public Entity {

    Q_OBJECT

public:
    virtual ~Contact();

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
     * Constrcutor is protected because only ContactSet objects should directly create
     * Contact objects.
     */
    explicit Contact(const Nepomuk::Resource &ncoPersonContact, const Nepomuk::Resource &ncoImAccount);

    /**
     * Construct an invalid Contact object
     */
    Contact();

private Q_SLOTS:
    /**
     * Updates the avatar and presence icon.
     */
    void updatePresenceIconAndAvatar();

private:
    Q_DISABLE_COPY(Contact);

    friend class ContactSet;

    class Private;
    Private * const d;

};

typedef QSharedPointer<Contact> ContactPtr;

}  // namespace KTelepathy

Q_DECLARE_METATYPE(QSharedPointer<KTelepathy::Contact>);


#endif  // Header guard
