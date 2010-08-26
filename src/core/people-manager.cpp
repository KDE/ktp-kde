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

#include "everyone-person-set.h"

#include <KDebug>
#include <KGlobal>

#include <Nepomuk/Resource>

#include <QtCore/QHash>

using namespace KTelepathy;


/****************************** PeopleManager::Private ********************************************/


class PeopleManager::Private {

public:
    explicit Private(PeopleManager *parent)
      : q(parent)
    {
        // Initiate the "me" PIMO:Person
        // FIXME: Need a standard way to get "me" from Nepomuk
        mePimoPerson = QUrl::fromEncoded("nepomuk:/myself");
    }

    PeopleManager * const q;

    QWeakPointer<EveryonePersonSet> everyonePersonSet;

    Nepomuk::Resource mePimoPerson;

    QHash<QUrl, QWeakPointer<Person> > personCache;

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

PersonSetPtr PeopleManager::everyone()
{
    kDebug();

    if (d->everyonePersonSet.isNull()) {
        QSharedPointer<EveryonePersonSet> everyonePersonSet(new EveryonePersonSet(d->mePimoPerson));
        d->everyonePersonSet = everyonePersonSet.toWeakRef();
        // Must return from here otherwise the only strong ref (everyonePersonSet above) goes out of
        // scope and the everyonePersonSet gets destroyed.
        return d->everyonePersonSet.toStrongRef();
    }

    return d->everyonePersonSet.toStrongRef();
}

PersonPtr PeopleManager::personForResource(const Nepomuk::Resource &resource)
{
    // If the Nepomuk::Resource does not have a valid URI, then return a null Person.
    if (resource.resourceUri().isEmpty()) {
        return QSharedPointer<Person>();
    }

    // Look in the cache to see if the person exists already.
    QWeakPointer<Person> weakPerson = d->personCache.value(resource.resourceUri());
    if (!weakPerson.isNull() && weakPerson.data()->isValid()) {
        return weakPerson.toStrongRef();
    }

    // Person is not in the cache. Create a new one and insert it into the cache
    QSharedPointer<Person> person(new Person(resource));

    if (person->isValid()) {
        d->personCache.insert(resource.resourceUri(), person);
        return person;
    }

    return QSharedPointer<Person>();
}


#include "people-manager.moc"

