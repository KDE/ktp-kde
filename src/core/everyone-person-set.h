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

#ifndef LIBKTELEPATHY_EVERYONE_PERSON_SET_H
#define LIBKTELEPATHY_EVERYONE_PERSON_SET_H

#include "person-set.h"

#include <kdemacros.h>

#include <Nepomuk/Query/Result>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QUrl>

namespace Nepomuk {
    class Resource;
}

class KDE_NO_EXPORT EveryonePersonSet : public PersonSet {

    Q_OBJECT

public:
    virtual ~EveryonePersonSet();

private Q_SLOTS:
    void onNewEntries(const QList<Nepomuk::Query::Result> &entries);
    void onEntriesRemoved(const QList<QUrl> &entries);

private:
    // Private constructor because this object should only be instantiated by PeopleManager
    explicit EveryonePersonSet(const Nepomuk::Resource &mePimoPerson, QObject *parent = 0);
    friend class PeopleManager;

    class Private;
    Private * const d;

};


#endif  // Header guard

