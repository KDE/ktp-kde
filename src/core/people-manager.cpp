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

#include "people-manager.h"

#include <KDebug>
#include <KGlobal>


/****************************** PeopleManager::Private ********************************************/


class PeopleManager::Private {

public:
    explicit Private(PeopleManager *parent)
      : q(parent)
    { }

    PeopleManager * const q;

};


/****************************** Global Static Stuff ***********************************************/


class PeopleManagerHelper
{
public:
    PeopleManagerHelper() : q(0) {}
    ~PeopleManagerHelper() {
        delete q;
    }
    PeopleManager *q;
};

K_GLOBAL_STATIC(PeopleManagerHelper, s_globalPeopleManager)


/****************************** PeopleManager *****************************************************/


PeopleManager *PeopleManager::instance()
{
    if (!s_globalPeopleManager->q) {
        new PeopleManager;
    }

    return s_globalPeopleManager->q;
}

PeopleManager::PeopleManager()
  : QObject(0),
    d(new PeopleManager::Private(this))
{
    kDebug();

    Q_ASSERT(!s_globalPeopleManager->q);
    s_globalPeopleManager->q = this;
}

PeopleManager::~PeopleManager()
{
    kDebug();

    delete d;
}


#include "people-manager.moc"

