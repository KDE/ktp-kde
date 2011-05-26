/*
 * This file is part of libktelepathy
 *
 * Copyright (C) 2010-2011 Collabora Ltd. <info@collabora.co.uk>
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

#include "ontologies/nco.h"
#include "ontologies/pimo.h"
#include "ontologies/telepathy.h"

#include <KDebug>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/ResourceTerm>
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
    {
        using namespace Nepomuk::Query;
        using namespace Nepomuk::Vocabulary;

        // Find grounding occurrences of ME
        ComparisonTerm meGroundingOccurrencesTerm(PIMO::groundingOccurrence(),
                                                  ResourceTerm(d->mePimoPerson));
        meGroundingOccurrencesTerm.setInverted(true);

        // Filter the grounding occurrences to include only those that are PersonContacts.
        AndTerm mePersonContactsTerm(ResourceTypeTerm(NCO::PersonContact()),
                                     meGroundingOccurrencesTerm);

        // Find all the IMAccounts of these person contacts
        ComparisonTerm meImAccountsTerm(NCO::hasIMAccount(),
                                        mePersonContactsTerm);
        meImAccountsTerm.setInverted(true);

        // Find all other-people's IM Accounts that are accessed by our IM Accounts.
        // We include everyone in this query, no matter whether there are presence publishing
        // links or not, because this PersonSet is the central PersonSet for all Telepathy enabled
        // people. It can be filtered later by proxy models.
        ComparisonTerm isAccessedByTerm(NCO::isAccessedBy(),
                                        meImAccountsTerm);
        isAccessedByTerm.setVariableName(QLatin1String("t"));

        // Ensure that the resource accessed through us are IM Accounts.
        AndTerm themImAccountsTerm(ResourceTypeTerm(NCO::IMAccount()),
                                   isAccessedByTerm);

        // Get the PersonContact associated with that IM Account.
        ComparisonTerm themPersonContactsTerm(NCO::hasIMAccount(), themImAccountsTerm);

        // Check that the PersonContact is a grounding occurrence of something.
        ComparisonTerm themGroundingOccurrencesTerm(PIMO::groundingOccurrence(),
                                                    AndTerm(ResourceTypeTerm(NCO::PersonContact()),
                                                                             themPersonContactsTerm));

        // Get the PIMO person which the PersonContact was a grounding occurrence of.
        Query query(AndTerm(ResourceTypeTerm(PIMO::Person()),
                            themGroundingOccurrencesTerm));

        bool queryResult = d->query->query(query);

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
        kDebug() << entry;
        removePerson(entry);
    }
}


#include "everyone-person-set.moc"

