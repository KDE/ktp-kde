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
        // subquery to match grouding occurrences of me
        ComparisonTerm goterm(PIMO::groundingOccurrence(),
                              ResourceTerm(d->mePimoPerson));
        goterm.setInverted(true);

        // combine that with only nco:PersonContacts
        AndTerm pcgoterm(ResourceTypeTerm(NCO::PersonContact()),
                         goterm);

        // now look for im accounts of those grounding occurrences (pcgoterm will become the subject of this comparison,
        // thus the comparison will match the im accounts)
        ComparisonTerm impcgoterm(NCO::hasIMAccount(),
                                  pcgoterm);
        impcgoterm.setInverted(true);

        // now look for all buddies of the accounts
        ComparisonTerm buddyTerm(Telepathy::isBuddyOf(),
                                 impcgoterm);
        // set the name of the variable (i.e. the buddies) to be able to match it later
        buddyTerm.setVariableName(QLatin1String("t"));

        // same comparison, other property, but use the same variable name to match them
        ComparisonTerm ppterm(Telepathy::publishesPresenceTo(),
                              ResourceTypeTerm(NCO::IMAccount()));
        ppterm.setVariableName(QLatin1String("t"));

        // combine both to complete the matching of the im account ?account
        AndTerm accountTerm(ResourceTypeTerm(NCO::IMAccount()),
                            buddyTerm, ppterm);

        // match the account and select it for the results
        ComparisonTerm imaccountTerm(NCO::hasIMAccount(), accountTerm);
        imaccountTerm.setVariableName(QLatin1String("account"));

        // the resulting person contact should be a grounding occurrence of something
        ComparisonTerm isgoterm(PIMO::groundingOccurrence(),
                                AndTerm(ResourceTypeTerm(NCO::PersonContact()), imaccountTerm));

        // Get the pimo:person of which the imaccountTerm is the grounding occurence.
        Query query(AndTerm(ResourceTypeTerm(PIMO::Person()),
                            isgoterm));

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
        removePerson(entry);
    }
}


#include "everyone-person-set.moc"

