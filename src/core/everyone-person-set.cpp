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

#include "everyone-person-set.h"

#include "people-manager.h"
#include "person.h"

#include "ontologies/pimo.h"

#include <KDebug>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/Result>

#include <QtCore/QList>
#include <QtCore/QUrl>

using namespace KTelepathy;

class EveryonePersonSet::Private {

public:
    Private(EveryonePersonSet *parent, const Nepomuk::Resource &me)
      : q(parent),
        mePimoPerson(me),
        query(0),
        peopleManager(PeopleManager::instance())
    {
    }

    EveryonePersonSet * const q;

    Nepomuk::Resource mePimoPerson;
    Nepomuk::Query::QueryServiceClient *query;

    PeopleManager *peopleManager;
};

EveryonePersonSet::EveryonePersonSet(const Nepomuk::Resource &mePimoPerson)
  : PersonSet(),
    d(new Private(this, mePimoPerson))
{
    kDebug();

    d->query = new Nepomuk::Query::QueryServiceClient(this);

    connect(d->query, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
            this, SLOT(onNewEntries(QList<Nepomuk::Query::Result>)));
    connect(d->query, SIGNAL(entriesRemoved(QList<QUrl>)),
            this, SLOT(onEntriesRemoved(QList<QUrl>)));

    // Get all PIMO:Persons
    // FIXME: Make this query only return people that can be contacted in some way by Telepathy.
    {
        using namespace Nepomuk::Query;

        Query query(ResourceTypeTerm(Nepomuk::Vocabulary::PIMO::Person()));

        bool queryResult = d->query->query(query);
        kDebug() << "Metacontact query result " << queryResult;

        if (!queryResult) {
            kWarning() << "Failed to query the Nepomuk database. QueryServiceClient may not be running";
        }
    }
}

EveryonePersonSet::~EveryonePersonSet()
{
    kDebug();
}

void EveryonePersonSet::onNewEntries(const QList<Nepomuk::Query::Result> &entries)
{
    kDebug();

    foreach (const Nepomuk::Query::Result &entry, entries) {
        kDebug() << entry.resource();
        PersonPtr person = d->peopleManager->personForResource(entry.resource());
        if (!person.isNull()) {
            addPerson(person);
        } else {
            kWarning() << "Got a Nepomuk Resource URI that is not a valid PIMO:Person";
        }
    }
}

void EveryonePersonSet::onEntriesRemoved(const QList<QUrl> &entries)
{
    kDebug();

    foreach (const QUrl &entry, entries) {
        removePerson(entry);
    }
}


#include "everyone-person-set.moc"

