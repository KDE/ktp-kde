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

#include "contact-set.h"
#include "entity.h"

#include <kdemacros.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <TelepathyQt4/SharedPtr>

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
class KDE_EXPORT Person : public ContactSet, public Entity {

    Q_OBJECT

//    Q_PROPERTY(ContactSetPtr contacts READ contacts /* TODO NOTIFY countChanged */)
    Q_PROPERTY(QPixmap avatar READ avatar NOTIFY avatarChanged) /* TODO With overlay? */
    Q_PROPERTY(QSet<QString> capabilities READ capabilities NOTIFY capabilitiesChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QSet<QString> groups READ groups NOTIFY groupsChanged)
    Q_PROPERTY(KIcon presenceIcon READ presenceIcon NOTIFY presenceIconChanged)

public:
    virtual ~Person();

    /**
     * Returns the avatar of this person
     */
    const QPixmap &avatar(bool withOverlay = false) const;

    /**
     * Returns the capabilities of this person
     */
    QSet<QString> capabilities() const;

    /**
     * Returns the display name of the person
     */
    QString displayName() const;

    /**
     * Returns the groups to which this person belongs
     */
    QSet<QString> groups() const;

    /**
     * Returns the presence Icon for this person
     */
    const KIcon &presenceIcon() const;

    /**
     * Returns the presence message
     */
    QString presenceMessage() const;

    /**
     * Returns the presence name
     */
    QString presenceName() const;

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
    void avatarChanged(const QPixmap &avatar);
    void avatarWithOverlayChanged(const QPixmap &avatar);
    void capabilitiesChanged(const QSet<QString> &capabilities);
    void displayNameChanged(const QString &displayName);
    void groupsChanged(const QSet<QString> &groups);
    void presenceIconChanged(const KIcon &presenceIcon);
    void presenceNameChanged(const QString &presenceName);
    void presenceMessageChanged(const QString &presenceMessage);

private Q_SLOTS:
    void onContactAdded(const KTelepathy::ContactPtr &contact);
    void onContactRemoved(const KTelepathy::ContactPtr &contact);
    void onNewEntries(const QList<Nepomuk::Query::Result> &entries);
    void onEntriesRemoved(const QList<QUrl> &entries);

    void updateAvatar();
    void updateAvatarWithOverlay();
    void updateCapabilities();
    void updateDisplayName();
    void updateGroups();
    void updatePresenceIcon();
    void updatePresenceMessage();
    void updatePresenceName();

private:
    Q_DISABLE_COPY(Person);

    friend class PeopleManager;

    class Private;
    Private * const d;

};

typedef Tp::SharedPtr<Person> PersonPtr;

}  // namespace KTelepathy

Q_DECLARE_METATYPE(Tp::SharedPtr<KTelepathy::Person>);

uint KDE_EXPORT qHash(const KTelepathy::PersonPtr &key);

#endif  // Header guard

