/*
 * This file is part of libktelepathy
 *
 * Copyright (C) 2010-2011 Collabora Ltd. <info@collabora.co.uk>
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

#include "contact-set.h"
#include "entity.h"

#include <kdemacros.h>

#include <QtCore/QObject>
#include <QtCore/QSet>

#include <TelepathyQt4/Presence>
#include <TelepathyQt4/SharedPtr>

namespace Nepomuk {
    class Resource;
}

class KIcon;
class QString;

namespace KTelepathy {

/**
 * @class Person
 *
 * This class is a subclass of Entity, representing a Person object (basically a wrapper around a
 * PIMO:Person in the Nepomuk store).
 */
class KDE_EXPORT Person : public ContactSet, public Entity {

    Q_OBJECT

    Q_PROPERTY(QString avatar READ avatar NOTIFY avatarChanged)
    Q_PROPERTY(QSet<QString> capabilities READ capabilities NOTIFY capabilitiesChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QSet<QString> groups READ groups NOTIFY groupsChanged)
    Q_PROPERTY(KIcon presenceIcon READ presenceIcon NOTIFY presenceIconChanged)
    Q_PROPERTY(QString presenceMessage READ presenceMessage NOTIFY presenceMessageChanged)
    Q_PROPERTY(QString presenceName READ presenceName NOTIFY presenceNameChanged)
    Q_PROPERTY(QString presenceType READ presenceType NOTIFY presenceTypeChanged)

public:
    virtual ~Person();

    /**
     * Returns the avatar of this person
     */
    const QString &avatar() const;

    /**
     * Returns the capabilities of this person
     */
    const QSet<QString> &capabilities() const;

    /**
     * Returns the display name of this person
     */
    const QString &displayName() const;

    /**
     * Returns the groups to which this person belongs
     */
    const QSet<QString> &groups() const;

    /**
     * Returns the presence Icon for this person
     */
    const KIcon &presenceIcon() const;

    /**
     * Returns the presence message for this person
     */
    const QString &presenceMessage() const;

    /**
     * Returns the presence name for this person
     */
    const QString &presenceName() const;

    /**
     * Returns the presence type (integer) for this person
     */
    Tp::ConnectionPresenceType presenceType() const;

protected:
    /**
     * Constrcutor is protected because only PeopleManager should directly create Person objects.
     */
    explicit Person(const Nepomuk::Resource &pimoPerson);

    /**
     * Construct an invalid Person object
     */
    Person();

Q_SIGNALS:
    /**
     * Emitted when the avatar has changed.
     */
    void avatarChanged(const QString &fileName);

    /**
     * Emitted when the capabilities have changed
     */
    void capabilitiesChanged(const QSet<QString> &capabilities);

    /**
     * Emitted when the display name has changed
     */
    void displayNameChanged(const QString &displayName);

    /**
     * Emitted when the groups have changed
     */
    void groupsChanged(const QSet<QString> &groups);

    /**
     * Emitted when the presence Icon has changed.
     */
    void presenceIconChanged(const KIcon &presenceIcon);

    /**
     * Emitted when the presence message has changed.
     */
    void presenceMessageChanged(const QString &presenceMessage);

    /**
     * Emitted when the presence name has changed.
     */
    void presenceNameChanged(const QString &presenceName);

    /**
     * Emitted when the presence type has changed.
     */
    void presenceTypeChanged(Tp::ConnectionPresenceType presenceType);

private Q_SLOTS:
    void onContactAdded(const KTelepathy::ContactPtr &contact);
    void onContactRemoved(const KTelepathy::ContactPtr &contact);
    void onNewEntries(const QList<Nepomuk::Query::Result> &entries);
    void onEntriesRemoved(const QList<QUrl> &entries);

    /**
     * Internal slot to handle avatar changes.
     */
    void updateAvatar();

    /**
     * Internal slot to handle capability changes.
     */
    void updateCapabilities();

    /**
     * Internal slot to handle display name changes.
     */
    void updateDisplayName();

    /**
     * Internal slot to handle group changes.
     */
    void updateGroups();

    /**
     * Internal slot to handle presence icon changes.
     */
    void updatePresenceIcon();

    /**
     * Internal slot to handle presence message changes.
     */
    void updatePresenceMessage();

    /**
     * Internal slot to handle presence name changes.
     */
    void updatePresenceName();

    /**
     * Internal slot to handle presence type changes.
     */
    void updatePresenceType();

private:
    Q_DISABLE_COPY(Person);

    friend class PeopleManager;
    friend class TestBackdoors;

    class Private;
    Private * const d;

};

typedef Tp::SharedPtr<Person> PersonPtr;

}  // namespace KTelepathy

Q_DECLARE_METATYPE(Tp::SharedPtr<KTelepathy::Person>);

uint KDE_EXPORT qHash(const KTelepathy::PersonPtr &key);


#endif  // Header guard

