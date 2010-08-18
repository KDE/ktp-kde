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

#include "person-set-model.h"

#include <KDebug>

using namespace KTelepathy;

/****************************************** Item class ********************************************/

class PersonSetModel::Item {

public:
    Item()
      : parent(0)
    { }

    ~Item()
    {
        Q_FOREACH (Item *item, children) {
            delete item;
        }
    }

    PersonPtr person;
    Item *parent;
    QList<Item*> children;

};

/***************************************** Private class ******************************************/

class PersonSetModel::Private {

public:
    explicit Private(PersonSetModel *parent)
      : q(parent),
        rootItem(new Item)
    {
    }

    ~Private()
    {
        delete rootItem;
    }

    PersonSetModel * const q;
    PersonSetPtr personSet;
    Item *rootItem;
};

/****************************************** Main class ********************************************/

PersonSetModel::PersonSetModel(QObject *parent)
 : QAbstractItemModel(parent),
   d(new Private(this))
{
    kDebug();
}

PersonSetModel::PersonSetModel(const PersonSetPtr &personSet, QObject *parent)
  : QAbstractItemModel(parent),
    d(new Private(this))
{
    kDebug();

    setPersonSet(personSet);
}

PersonSetModel::~PersonSetModel()
{
}

void PersonSetModel::setPersonSet(const PersonSetPtr &personSet)
{
    // Indicate to the views that all rows will be removed, and remove them
    if (!d->rootItem->children.isEmpty()) {
        QList<Item*>::iterator i = d->rootItem->children.begin();
        while (i != d->rootItem->children.end()) {
            onPersonRemoved((*i)->person);
        }
        Q_ASSERT(d->rootItem->children.isEmpty());
        d->rootItem->children.clear();
    }

    // Set the new PersonSet, and connect to its add/remove signals
    d->personSet = personSet;
    connect(d->personSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(onPersonAdded(KTelepathy::PersonPtr)));
    connect(d->personSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(onPersonRemoved(KTelepathy::PersonPtr)));

    // Simulate calling personAdded for each of the people already in the list
    Q_FOREACH (PersonPtr person, d->personSet->people()) {
        onPersonAdded(person);
    }
}

PersonSetPtr PersonSetModel::personSet() const
{
    return d->personSet;
}

int PersonSetModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    // All items have the same number of columns
    return 1;
}

QVariant PersonSetModel::data(const QModelIndex &index, int role) const
{
    // Only column 0 is valid.
    if (index.column() != 0) {
        return QVariant();
    }

    // Check the parent of this item is the root item (invalid QModelIndex)
    if (index.parent().isValid()) {
        return QVariant();
    }

    // Check the row ID is within the valid range.
    if (index.row() >= d->rootItem->children.size()) {
        return QVariant();
    }

    Item *item = d->rootItem->children.value(index.row());

    Q_ASSERT(item);

    QVariant data;
    switch(role)
    {
    case Qt::DisplayRole:
        data.setValue<PersonPtr>(item->person);
        break;
    default:
        break;
    }

    return data;
}

Qt::ItemFlags PersonSetModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

QVariant PersonSetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);

    return QVariant();
}

QModelIndex PersonSetModel::index(int row, int column, const QModelIndex &parent) const
{
    // this is a list, not a tree, so parent must be QModelIndex() (root item)
    if (parent.isValid()) {
        return QModelIndex();
    }

    // 1 column only, so any other column is invalid
    if (column != 0) {
        return QModelIndex();
    }

    // Check the row is within the bounds of the list
    if (row >= d->rootItem->children.size() || row < 0) {
        return QModelIndex();
    }

    // Return the index to the item.
    return createIndex(row, column, d->rootItem->children.at(row));
}

QModelIndex PersonSetModel::parent(const QModelIndex &index) const
{
    // List model, not tree model, so all items have root-item as parent.
    return QModelIndex();
}

int PersonSetModel::rowCount(const QModelIndex &parent) const
{
    // List model, so only the root item has children.
    if (parent.isValid()) {
        return 0;
    }

    return d->rootItem->children.size();
}

void PersonSetModel::onPersonAdded(const KTelepathy::PersonPtr &person)
{
    // Notify the views that we are adding rows
    beginInsertRows(QModelIndex(), d->rootItem->children.size(), d->rootItem->children.size());

    // Create a new item for the new row
    Item *item = new Item;
    item->parent = d->rootItem;
    item->person = person;

    // TODO: Connect to signals that say the item has changed in some way (once those signals exist)

    // Add the new item to the model data.
    d->rootItem->children.append(item);

    // Notify views that we have finished adding rows
    endInsertRows();
}

void PersonSetModel::onPersonRemoved(const KTelepathy::PersonPtr &person)
{
    // Get the row number of the item we are going to remove.
    int row = -1;
    Q_FOREACH (Item *item, d->rootItem->children) {
        if (item->person = person) {
            row = d->rootItem->children.indexOf(item);
            break;
        }
    }

    Q_ASSERT(row >= 0);
    if (row >= 0) {
        kWarning() << "Tried to remove a person from the model which is not in the model.";
        return;
    }

    // Notify the views that we are going to remove a row.
    beginRemoveRows(QModelIndex(), row, row);

    // Actually remove the row from the model data, and delete the item
    delete d->rootItem->children.value(row);
    d->rootItem->children.removeAt(row);

    // TODO: Disconnect from the signals that say the item has changed in some way (once they exist)

    // Notify the views that we have finished removing rows.
    endRemoveRows();
}


#include "person-set-model.moc"

