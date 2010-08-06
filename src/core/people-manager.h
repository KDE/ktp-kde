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

#ifndef LIBKTELEPATHY_PEOPLE_MANAGER_H
#define LIBKTELEPATHY_PEOPLE_MANAGER_H

#include "person.h"
#include "person-set.h"

#include <kdemacros.h>

#include <QtCore/QObject>

/**
 * @class PeopleManager
 *
 * This class represents the central store of all people. It is the starting point for all
 * applications wishing to interact with People.
 */
class KDE_EXPORT PeopleManager : public QObject {

    Q_OBJECT

public:
    virtual ~PeopleManager();

    static PeopleManager *instance();

    PersonSetPtr everyone();

    PersonPtr personForResource(const Nepomuk::Resource &resource);

private:
    PeopleManager();

    Q_DISABLE_COPY(PeopleManager);

    class Private;
    Private * const d;

};


#endif  // Header guard

