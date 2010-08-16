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

#include "accounts-model.h"
#include "abstract-tree-item.h"
#include "account-item.h"

// Nepomuk Vocabulary URIs
#include "ontologies/pimo.h"
#include "ontologies/nco.h"
#include "ontologies/telepathy.h"

// Nepomuk Resources
#include "ontologies/imaccount.h"
#include "ontologies/person.h"
#include "ontologies/personcontact.h"

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Variant>

#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/NegationTerm>
#include <Nepomuk/Query/Result>

#include <KDebug>
#include <KMessageBox>

using namespace KTelepathy;

AccountsModel::AccountsModel(QObject *parent)
 : QAbstractItemModel(parent),
   m_rootItem(0)
{
    kDebug();

    // Create the root Item.
    m_rootItem = new AbstractTreeItem;

    // FIXME: Get the Nepomuk Resource for myself in the standardised way, once it is standardised.
    Nepomuk::Resource me(QUrl::fromEncoded("nepomuk:/myself"));

    m_accountsQuery = new Nepomuk::Query::QueryServiceClient(this);

    connect(m_accountsQuery, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
            this, SLOT(onAccountsQueryNewEntries(QList<Nepomuk::Query::Result>)));
    connect(m_accountsQuery, SIGNAL(entriesRemoved(QList<QUrl>)),
            this, SLOT(onAccountsQueryEntriesRemoved(QList<QUrl>)));

    // Get all Telepathy Accounts belonging to myself Person
    {
        using namespace Nepomuk::Query;
        using namespace Nepomuk::Vocabulary;

        // subquery to match grouding occurrences of me
        ComparisonTerm goterm(PIMO::groundingOccurrence(),
                              ResourceTerm(me));
        goterm.setInverted(true);
        // combine that with only nco:PersonContacts
        AndTerm pcgoterm(ResourceTypeTerm(NCO::PersonContact()),
                         goterm);

        // now look for im accounts of those grounding occurrences
        ComparisonTerm impcgoterm(NCO::hasIMAccount(),
                                  pcgoterm);
        impcgoterm.setInverted(true);

        // get only the resources that are of type IMAccount
        AndTerm myimterm(ResourceTypeTerm(NCO::IMAccount()),
                         impcgoterm);

        Query query(myimterm);

        bool queryResult = m_accountsQuery->query(query);

        kDebug() << "Account query result " << queryResult;
        if (!queryResult) {
            KMessageBox::error(0, i18n("It was not possible to query Nepomuk database. Please check your "
                                       "installation and make sure Nepomuk is running."));
        }
    }
}


AccountsModel::~AccountsModel()
{
    kDebug();
}


void AccountsModel::onAccountsQueryNewEntries(const QList<Nepomuk::Query::Result> &entries)
{
    kDebug();
    // Iterate over all the IMAccounts/PersonContacts found.
    foreach (const Nepomuk::Query::Result &result, entries) {
        Nepomuk::IMAccount foundIMAccount(result.resource());
        kDebug() << "New resource added: " << foundIMAccount;

        // Create an AccountItem to hold this item.
        AccountItem* accountItem = new AccountItem(foundIMAccount, 0);
        accountItem->setParentItem(m_rootItem);
        m_rootItem->appendChildItem(accountItem);
        connect(accountItem, SIGNAL(dirty()), SLOT(onItemDirty()));
    }

    reset();
}

void AccountsModel::onAccountsQueryEntriesRemoved(const QList< QUrl > &entries)
{
    kDebug();
    // Find and remove
    foreach (const QUrl &url, entries) {
        kDebug() << "Attempting to remove resource " << url;
        foreach (const QModelIndex &index, findAccounts(url)) {
            kDebug() << "Found matching item at" << index;
            AbstractTreeItem *abstractItem = static_cast<AbstractTreeItem*>(index.internalPointer());
            AccountItem *accountItem = dynamic_cast<AccountItem*>(abstractItem);

            if (accountItem) {
                m_rootItem->removeChildItem(accountItem);
                delete accountItem;
            }
        }
    }

    reset();
}

QModelIndexList AccountsModel::findAccounts(const QUrl& resource)
{
    return findChildrenAccounts(resource, QModelIndex());
}

QModelIndexList AccountsModel::findChildrenAccounts(const QUrl& resource, const QModelIndex& parent)
{
    QModelIndexList list;

    kDebug() << "Finding accounts in " << parent << rowCount(parent) << columnCount(parent);

    for (int i = 0; i < rowCount(parent); ++i) {
        for (int j = 0; j < columnCount(parent); ++j) {
            QModelIndex current = index(i, j, parent);

            if (current.data(IMAccountResourceRole) == resource) {
                list << current;
            }

            // Iterate over children, if any
            if (hasChildren(current)) {
                list << findChildrenAccounts(resource, current);
            }
        }
    }

    return list;
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    // All items have the same number of columns
    return 1;
}


QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    // Only column 0 is valid.
    if (index.column() != 0) {
        return QVariant();
    }

    // Check what type of item we have here.
    AbstractTreeItem *abstractItem = static_cast<AbstractTreeItem*>(index.internalPointer());
    AccountItem *accountItem = dynamic_cast<AccountItem*>(abstractItem);

    if (accountItem) {

        QVariant data;

        switch(role)
        {
        case Qt::DisplayRole:
            data.setValue<QString>(accountItem->accountIdentifier());
            break;
        case Qt::DecorationRole:
            data.setValue<QIcon>(accountItem->presenceIcon());
            break;
        case AccountsModel::PresenceTypeRole:
            data.setValue<qint64>(accountItem->presenceType());
            break;
/*TODO        case AccountsModel::AvatarRole:
            data.setValue<QPixmap>(accountItem->avatar());
            break;*/
        case AccountsModel::IMAccountResourceRole:
            data.setValue<QUrl>(accountItem->imAccount().resourceUri());
            break;
        default:
            break;
        }

        return data;
    }

    Q_ASSERT(false);
    kWarning() << "Model Index pointer is of invalid type.";

    return QVariant();
}


Qt::ItemFlags AccountsModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}


QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return QVariant(i18n("Account Name"));
    }

    return QVariant();
}


QModelIndex AccountsModel::index(int row, int column, const QModelIndex &parent) const
{
    // 1 column list, so invalid index if the column is not 1.
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }

    // Get the parent item.
    AbstractTreeItem *parentItem = item(parent);

    // Get all the parent's children.
    QList<AbstractTreeItem*> children = parentItem->childItems();

    // Check the row doesn't go beyond the end of the list of children.
    if (row >= children.length()) {
        return QModelIndex();
    }

    // Return the index to the item.
    return createIndex(row, column, children.at(row));
}


QModelIndex AccountsModel::parent(const QModelIndex &index) const
{
    // If the index is invalid, return an invalid parent index.
    if (!index.isValid()) {
        return QModelIndex();
    }

    // Get the item we have been passed, and it's parent
    AbstractTreeItem *childItem = item(index);
    AbstractTreeItem *parentItem = childItem->parentItem();

    // If the parent is the root item, then the parent index of the index we were passed is
    // by definition an invalid index.
    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    // The parent of the item is not the root item, meaning that the parent must have a parent too.
    AbstractTreeItem *parentOfParentItem = parentItem->parentItem();

    // As stated in the previous comment, something is really wrong if it doesn't have a parent.
    Q_ASSERT(parentOfParentItem);
    if (!parentOfParentItem) {
        kWarning() << "Impossible parent situation occurred!";
        return createIndex(0, 0, parentItem);
    }

    // Return the model index of the parent item.
    return createIndex(parentOfParentItem->childItems().lastIndexOf(parentItem), 0, parentItem);
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
    // If the parent is invalid, then this request is for the root item.
    if (!parent.isValid()) {
        return m_rootItem->childItems().length();
    }

    // Get the item from the internal pointer of the ModelIndex.
    AbstractTreeItem *item = static_cast<AbstractTreeItem*>(parent.internalPointer());

    // If the item is valid, return the number of children it has.
    if (item) {
        return item->childItems().length();
    }

    // Otherwise, return 0
    return 0;
}

void AccountsModel::onItemDirty()
{
    AccountItem *item = qobject_cast<AccountItem*>(sender());

//    Q_ASSERT(item);
    if (!item) {
        kWarning() << "Invalid sender.";
    }

    reset();

    // FIXME: Port this stuff to new tree structure.
   // QModelIndex itemIndex = index(m_accountItems.indexOf(item), 0, QModelIndex());
   // Q_EMIT dataChanged(itemIndex, itemIndex);
}

AbstractTreeItem* AccountsModel::item(const QModelIndex &index) const
{
    if (index.isValid()) {
        AbstractTreeItem *item = static_cast<AbstractTreeItem*>(index.internalPointer());
         if (item) {
             return item;
         }
     }

     return m_rootItem;
}


#include "accounts-model.moc"


