/*
* Copyright (C) 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#ifndef LIBKTELEPATHY_ACCOUNTS_MODEL_H
#define LIBKTELEPATHY_ACCOUNTS_MODEL_H

#include <kdemacros.h>

#include <QtCore/QAbstractItemModel>

namespace Nepomuk {
    namespace Query {
        class QueryServiceClient;
        class Result;
    }
}

class AbstractTreeItem;

class KDE_EXPORT AccountsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum {
        PresenceTypeRole = Qt::UserRole,
        AvatarRole,
        IMAccountResourceRole
    };

    explicit AccountsModel(QObject *parent = 0);
    virtual ~AccountsModel();

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
    virtual QModelIndexList findAccounts(const QUrl &resource);

private Q_SLOTS:
    void onItemDirty();

    void onAccountsQueryNewEntries(const QList<Nepomuk::Query::Result>&);
    void onAccountsQueryEntriesRemoved(const QList<QUrl>&);

private:
    Q_DISABLE_COPY(AccountsModel);

    QModelIndexList findChildrenAccounts(const QUrl &resource, const QModelIndex &parent);

    AbstractTreeItem* item(const QModelIndex &index) const;

    AbstractTreeItem* m_rootItem;

    Nepomuk::Query::QueryServiceClient* m_accountsQuery;
};

#endif // LIBKTELEPATHY_ACCOUNTS_MODEL_H