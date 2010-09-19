/*
 * This file is part of telepathy-contactslist-prototype
 *
 * Copyright (C) 2010 Collabora Ltd. <info@collabora.co.uk>
 *   @Author George Goldberg <george.goldberg@collabora.co.uk>
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

#ifndef LIBKTELEPATHY_PERSON_SET_MODEL_H
#define LIBKTELEPATHY_PERSON_SET_MODEL_H

#include "person-set.h"

#include <kdemacros.h>

#include <QtCore/QAbstractItemModel>

namespace KTelepathy {

class KDE_EXPORT PersonSetModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum {
        PersonRole = Qt::UserRole,
        GroupsRole,
        CapabilitiesRole,
        AvatarRole,
        PresenceNameRole,
        PresenceMessageRole,
    };

    explicit PersonSetModel(QObject *parent = 0);
    explicit PersonSetModel(const PersonSetPtr &personSet, QObject *parent = 0);
    virtual ~PersonSetModel();

    void setPersonSet(const PersonSetPtr &personSet);
    PersonSetPtr personSet() const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row,
                              int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

private Q_SLOTS:
    void onPersonAdded(const KTelepathy::PersonPtr &person);
    void onPersonRemoved(const KTelepathy::PersonPtr &person);
    void onPersonDataChanged();

private:
    Q_DISABLE_COPY(PersonSetModel);

    class Private;
    Private * const d;

    class Item;
};

}  // Namespace KTelepathy

Q_DECLARE_METATYPE(QSet<QString>);


#endif // LIBKTELEPATHY_PERSON_SET_MODEL_H

