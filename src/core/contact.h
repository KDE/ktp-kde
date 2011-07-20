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

#ifndef LIBKTELEPATHY_CONTACT_H
#define LIBKTELEPATHY_CONTACT_H

#include "entity.h"
#include "nepomuk-signal-watcher.h"

#include <kdemacros.h>

#include <QtCore/QObject>
#include <QtCore/QSet>

#include <TelepathyQt4/Presence>
#include <TelepathyQt4/RefCounted>
#include <TelepathyQt4/SharedPtr>

namespace Nepomuk {
    class Resource;
}

class KIcon;

class QString;

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
class KDE_EXPORT Contact : public QObject, public Entity, public Tp::RefCounted, public NepomukSignalWatcher::Watcher {

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
    virtual ~Contact();

    /**
     * Returns the avatar of this contact
     */
    const QString &avatar() const;

    /**
     * Returns the capabilities of this contact
     */
    const QSet<QString> &capabilities() const;

    /**
     * Returns the display name of the contact
     */
    const QString &displayName() const;

    /**
     * Returns the groups to which this contact belongs
     */
    const QSet<QString> &groups() const;

    /**
     * Returns the presence Icon for this contact
     */
    const KIcon &presenceIcon() const;

    /**
     * Returns the presence name for this contact
     */
    const QString &presenceName() const;

    /**
     * Returns the presence message for this contact
     */
    const QString &presenceMessage() const;

    /**
     * Returns the presence type for this contact
     */
    Tp::ConnectionPresenceType presenceType() const;

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

    /**
     * Called whenever a statement is added to a resource that we are watching.
     * 
     * Reimplemented from NepomukSignalWatcher::Watcher.
     */
    virtual void onStatementAdded(const Soprano::Statement &statement);

    /**
     * Called whenever a statement is removed from a resource that we are watching.
     * 
     * Reimplemented from NepomukSignalWatcher::Watcher
     */
    virtual void onStatementRemoved(const Soprano::Statement &statement);

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
    Q_DISABLE_COPY(Contact);

    friend class ContactSet;
    friend class Person;
    friend class TestBackdoors;

    class Private;
    Private * const d;

};

typedef Tp::SharedPtr<Contact> ContactPtr;

}  // namespace KTelepathy

Q_DECLARE_METATYPE(Tp::SharedPtr<KTelepathy::Contact>);

uint KDE_EXPORT qHash(const KTelepathy::ContactPtr &key);


#endif  // Header guard

