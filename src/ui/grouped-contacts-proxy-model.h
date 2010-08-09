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

#ifndef LIBKTELEPATHY_GROUPED_CONTACTS_PROXY_MODEL_H
#define LIBKTELEPATHY_GROUPED_CONTACTS_PROXY_MODEL_H

#include <kdemacros.h>

#include <abstract-tree-item.h>

#include <QtGui/QAbstractProxyModel>

class ContactsListModel;

class KDE_EXPORT GroupedContactsProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    GroupedContactsProxyModel(QObject* parent = 0);
    virtual ~GroupedContactsProxyModel();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    virtual QItemSelection mapSelectionFromSource(const QItemSelection &sourceSelection) const;
    virtual QItemSelection mapSelectionToSource(const QItemSelection &proxySelection) const;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

    virtual void setSourceModel(QAbstractItemModel *sourceModel);

private Q_SLOTS:
    void onSourceRowsInserted(const QModelIndex &parent, int start, int end);
    void onSourceRowsRemoved(const QModelIndex &parent, int start, int end);
    void onSourceReset();

private:
    class Item : public AbstractTreeItem {
    public:
        QString name;
        QModelIndex sourceIndex;

        Item() { }
    };

    Item *item(const QModelIndex &index) const;

    ContactsListModel *m_sourceModel;
    Item *m_rootItem;

};


#endif // LIBKTELEPATHY_GROUPED_CONTACTS_PROXY_MODEL_H

