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

#ifndef LIBKTELEPATHY_ENTITY_H
#define LIBKTELEPATHY_ENTITY_H

#include <kdemacros.h>

#include <Nepomuk/Resource>

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>

/**
 * @class Entity
 *
 * Provides the base class for objects that are simply backed by a Nepomuk resource, such as
 * PIMO:Person based Person objects and NCO:Contact based Contact objects.
 */
class KDE_EXPORT Entity : public QObject {

    Q_OBJECT

public:
    explicit Entity(const Nepomuk::Resource &resource);
    virtual ~Entity();

    /**
     * @brief indicates whether the entity is valid.
     * @return true if the entity is valid, otherwise false.
     *
     * This indicates if the entity is valid. A valid entity means that the resource which it
     * contains is of the appropriate type (e.g. PIMO:Person for a Person entity). If an Entity is
     * valid, it is possible to actually make use of it for something, and resource will return
     * the resource this entity wraps.
     */
    bool isValid() const;

    /**
     * @brief get the resource this entity wraps.
     * @return the resource this entity wraps.
     *
     * This method will return the resource this entity wraps if the entity is valid. If the entity
     * is not valid, it will always return an invalid resource even if the entity was constructed
     * with a valid resource.
     *
     * If the Nepomuk resource this entity wraps is deleted from the Nepomuk store, the corresponding
     * Nepomuk::Resource object will have all its properties immediately cleared. In order to get
     * the URI of the wrapped Nepomuk resource in a way which guarantees to provide it even if the
     * resource is removed from the nepomuk store, call resourceUri() instead.
     */
    Nepomuk::Resource resource() const;

    /**
     * @brief returns the URI of the resource this entity wraps, even if the resource no longer exists.
     * @return the URI of the wrapped resource.
     *
     * This method returns the URI of the wrapped resource, even if that resource has been removed
     * from the Nepomuk storage, and therefore the properties of the corresponding Nepomuk::Resource
     * object have been reset.
     */
    QUrl resourceUri() const;

    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;

protected:
    /**
     * Sub-classes use this method to indicate if the Entity should be valid or not. It should be
     * called by subclasses in the constructor after they have discovered whether the resource
     * provided to the constructor is of a valid type for the type of entity (e.g. a PIMO:Person
     * resource for a Person entity).
     */
    void setValid(bool valid);

private:
    class Private;
    Private * const d;

};

typedef QSharedPointer<Entity> EntityPtr;

uint KDE_EXPORT qHash(Entity key);
uint KDE_EXPORT qHash(EntityPtr key);

#endif  // Header guard

