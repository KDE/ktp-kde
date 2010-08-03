/*
 * This file is part of people
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

#ifndef LIBKTELEPATHY_PEOPLE_MANAGER_H
#define LIBKTELEPATHY_PEOPLE_MANAGER_H

#include <KDebug>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QWeakPointer>

/**
 * @class PeopleManager
 *
 * This class represents the central store of all people. It is the starting point for all
 * applications wishing to interact with People.
 */
class PeopleManager : public QObject {

    Q_OBJECT

public:
    explicit PeopleManager(QObject *parent = 0);
    virtual ~PeopleManager();

private:
    class Private;
    Private * const d;

};


#endif  // Header guard

