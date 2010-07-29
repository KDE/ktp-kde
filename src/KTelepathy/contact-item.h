/*
 * This file is part of telepathy-contactslist-prototype
 *
 * Copyright (C) 2009-2010 Collabora Ltd. <info@collabora.co.uk>
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

#ifndef LIBKTELEPATHY_CONTACT_ITEM_H
#define LIBKTELEPATHY_CONTACT_ITEM_H

#include <kdemacros.h>

#include "abstract-tree-item.h"
#include "nepomuk-signal-watcher.h"

#include "imaccount.h"
#include "personcontact.h"

#include <KIcon>

#include <QObject>

class KDE_EXPORT ContactItem : public QObject,
                               public AbstractTreeItem,
                               protected NepomukSignalWatcher::Watcher
{

    Q_OBJECT

public:
    ContactItem(Nepomuk::PersonContact personContact,
                Nepomuk::IMAccount imAccount,
                QObject *parent = 0);
    ~ContactItem();

    QString displayName() const;
    QString accountIdentifier() const;
    QUrl isBuddyOf() const;
    const KIcon& presenceIcon() const;
    const QPixmap& avatar() const;
    qint64 presenceType() const;
    QStringList groups() const;

    Nepomuk::PersonContact personContact() const;
    Nepomuk::IMAccount imAccount() const;

    virtual void onStatementAdded(const Soprano::Statement &statement);

Q_SIGNALS:
    void dirty();

private Q_SLOTS:
    void updatePresenceIcon();

private:
    Q_DISABLE_COPY(ContactItem);

    Nepomuk::PersonContact m_personContact;
    Nepomuk::IMAccount m_imAccount;

    KIcon *m_presenceIcon;
    QPixmap m_pixmap;
};


#endif // LIBKTELEPATHY_CONTACT_ITEM_H
