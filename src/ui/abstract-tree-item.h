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

#ifndef LIBKTELEPATHY_ABSTRACT_TREE_ITEM_H
#define LIBKTELEPATHY_ABSTRACT_TREE_ITEM_H

#include <kdemacros.h>

#include <QtCore/QList>

namespace KTelepathy {

class KDE_EXPORT AbstractTreeItem
{
public:
    AbstractTreeItem();
    virtual ~AbstractTreeItem();

    virtual void appendChildItem(AbstractTreeItem *child);
    virtual void removeChildItem(AbstractTreeItem *child);

    virtual void setParentItem(AbstractTreeItem *parent);

    QList<AbstractTreeItem*> childItems() const;
    AbstractTreeItem *parentItem() const;

private:
    QList<AbstractTreeItem*> m_children;
    AbstractTreeItem *m_parent;
};

} // namespace KTelepathy

#endif // LIBKTELEPATHY_ABSTRACT_TREE_ITEM_H

