/*
 * This file is part of libktelepathy
 *
 * Copyright (C) 2009-2010 Collabora Ltd. <info@collabora.co.uk>
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

#include "people-manager-test.h"

#include "people-manager.h"
#include "person.h"

#include "ontologies/pimo.h"

#include <KDebug>

#include <Nepomuk/Thing>

#include <QTest>


PeopleManagerTest::PeopleManagerTest(QObject* parent)
    : TelepathyBaseTest(parent)
{

}
PeopleManagerTest::~PeopleManagerTest()
{

}

void PeopleManagerTest::initTestCase()
{
    initTestCaseImpl();
}

void PeopleManagerTest::init()
{

}

void PeopleManagerTest::testPersonFromResource()
{
    kDebug();

    // Nepomuk resources for this test
    Nepomuk::Resource relevantPersonResource(QUrl::fromEncoded("nepomuk:/person-from-resource-test-person"));
    Nepomuk::Resource irrelevantThingResource(QUrl::fromEncoded("nepomuk:/person-from-resource-test-random-thing"));
    Nepomuk::Resource nonExistantResource(QUrl::fromEncoded("nepomuk:/person-from-resource-test-non-existant-resource"));

    // Insert a test person into the Nepomuk store.
    Nepomuk::Thing thing1(relevantPersonResource);
    QVERIFY(!thing1.exists());
    thing1.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing1.exists());
    QVERIFY(thing1.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Insert another resource into the Nepomuk store which is completely irrelevant
    Nepomuk::Thing thing2(irrelevantThingResource);
    QVERIFY(!thing2.exists());
    thing2.addType(Nepomuk::Vocabulary::PIMO::City());
    QVERIFY(thing2.exists());
    QVERIFY(thing2.hasType(Nepomuk::Vocabulary::PIMO::City()));
    QVERIFY(!thing2.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Check that the "non-existant" resource doesn't exist.
    QVERIFY(!nonExistantResource.exists());

    // Get the People Manager instance
    PeopleManager *pm = PeopleManager::instance();
    QVERIFY(pm);

    // Check that a suitable Nepomuk Resource gives us a valid person
    PersonPtr person1 = pm->personForResource(relevantPersonResource);
    QVERIFY(!person1.isNull());
    QVERIFY(person1->isValid());
    QCOMPARE(person1->resource(), relevantPersonResource);

    // Check that we get a null PersonPtr for an irrelevant Nepomuk Resource
    PersonPtr person2 = pm->personForResource(irrelevantThingResource);
    QVERIFY(person2.isNull());

    // Check that we get a null PersonPtr for a non-existant Nepomuk Resource
    PersonPtr person3 = pm->personForResource(nonExistantResource);
    QVERIFY(person3.isNull());

    // Cleanup: remove our three created resources
    relevantPersonResource.remove();
    irrelevantThingResource.remove();
    nonExistantResource.remove();

    QVERIFY(!relevantPersonResource.exists());
    QVERIFY(!irrelevantThingResource.exists());
    QVERIFY(!nonExistantResource.exists());
}

void PeopleManagerTest::testEveryone()
{
    kDebug();

    // Set up member vars used for this test
    m_everyonePerson1Added = false;
    m_everyonePerson2Added = false;
    m_everyonePerson3Added = false;

    // Prepopulate Nepomuk with a load of misc
    m_everyonePerson1 = QUrl::fromEncoded("nepomuk:/everyone-test-1");
    m_everyonePerson2 = QUrl::fromEncoded("nepomuk:/everyone-test-2");
    m_everyonePerson3 = QUrl::fromEncoded("nepomuk:/everyone-test-3");

    Nepomuk::Thing thing1(m_everyonePerson1);
    QVERIFY(!thing1.exists());
    thing1.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing1.exists());
    QVERIFY(thing1.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    Nepomuk::Thing thing2(m_everyonePerson2);
    QVERIFY(!thing2.exists());
    thing2.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing2.exists());
    QVERIFY(thing2.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    Nepomuk::Thing thing3(m_everyonePerson3);
    QVERIFY(!thing3.exists());
    thing3.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing3.exists());
    QVERIFY(thing3.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Get the People Manager instance
    PeopleManager *pm = PeopleManager::instance();
    QVERIFY(pm);

    // Get the PersonSet containing Everyone
    m_everyonePersonSet = pm->everyone();
    QVERIFY(!m_everyonePersonSet.isNull());

    // The person set should be empty to begin with
    QCOMPARE(m_everyonePersonSet->people().size(), 0);

    // Connect to the PersonSet's signals
    connect(m_everyonePersonSet.data(),
            SIGNAL(personAdded(PersonPtr)),
            SLOT(everyoneOnPersonAdded1(PersonPtr)));
    connect(m_everyonePersonSet.data(),
            SIGNAL(personRemoved(PersonPtr)),
            SLOT(everyoneOnPersonRemoved1(PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_everyonePersonSet->people().size(), 3);

    // Disconnect the signals from part one of the test.
    disconnect(m_everyonePersonSet.data(),
               SIGNAL(personAdded(PersonPtr)),
               this,
               SLOT(everyoneOnPersonAdded1(PersonPtr)));
    disconnect(m_everyonePersonSet.data(),
               SIGNAL(personRemoved(PersonPtr)),
               this,
               SLOT(everyoneOnPersonRemoved1(PersonPtr)));

    // Next, we add a new person to Nepomuk and see if the set picks it up.
    m_everyonePerson4 = QUrl::fromEncoded("nepomuk:/everyone-test-4");
    Nepomuk::Thing thing4(m_everyonePerson4);
    QVERIFY(!thing4.exists());
    thing4.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing4.exists());
    QVERIFY(thing4.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Connect to the signals
    connect(m_everyonePersonSet.data(),
            SIGNAL(personAdded(PersonPtr)),
            SLOT(everyoneOnPersonAdded2(PersonPtr)));
    connect(m_everyonePersonSet.data(),
            SIGNAL(personRemoved(PersonPtr)),
            SLOT(everyoneOnPersonRemoved2(PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_everyonePersonSet->people().size(), 4);

    // Disconnect the signals from part two of the test.
    disconnect(m_everyonePersonSet.data(),
               SIGNAL(personAdded(PersonPtr)),
               this,
               SLOT(everyoneOnPersonAdded2(PersonPtr)));
    disconnect(m_everyonePersonSet.data(),
               SIGNAL(personRemoved(PersonPtr)),
               this,
               SLOT(everyoneOnPersonRemoved2(PersonPtr)));

    // Next, remove a person from Nepomuk to see if it picks it up.
    m_everyonePerson2.remove();

    // Connect to the signals
    connect(m_everyonePersonSet.data(),
            SIGNAL(personAdded(PersonPtr)),
            SLOT(everyoneOnPersonAdded3(PersonPtr)));
    connect(m_everyonePersonSet.data(),
            SIGNAL(personRemoved(PersonPtr)),
            SLOT(everyoneOnPersonRemoved3(PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_everyonePersonSet->people().size(), 3);

    // Disconnect the signals from part three of the test.
    disconnect(m_everyonePersonSet.data(),
               SIGNAL(personAdded(PersonPtr)),
               this,
               SLOT(everyoneOnPersonAdded2(PersonPtr)));
    disconnect(m_everyonePersonSet.data(),
               SIGNAL(personRemoved(PersonPtr)),
               this,
               SLOT(everyoneOnPersonRemoved2(PersonPtr)));

    // Check that the everyonePersonSet is cached properly.
    PersonSetPtr personSet = pm->everyone();
    QCOMPARE(personSet, m_everyonePersonSet);

    // Check the cache gets cleared properly
    m_everyonePersonSet.clear();
    personSet.clear();

    // When the cached everyonePersonSet has been cleared, the new one should be empty until the
    // main loop is run.
    personSet = pm->everyone();
    QCOMPARE(personSet->people().size(), 0);

    personSet.clear();

    // Cleanup this test
    m_everyonePerson1.remove();
    m_everyonePerson3.remove();
    m_everyonePerson4.remove();
}

void PeopleManagerTest::everyoneOnPersonAdded1(const PersonPtr &person)
{
    kDebug();

    // See which person was added.
    if (person->resource() == m_everyonePerson1) {
        QVERIFY(!m_everyonePerson1Added);
        m_everyonePerson1Added = true;
    } else if (person->resource() == m_everyonePerson2) {
        QVERIFY(!m_everyonePerson2Added);
        m_everyonePerson2Added = true;
    } else if (person->resource() == m_everyonePerson3) {
        QVERIFY(!m_everyonePerson3Added);
        m_everyonePerson3Added = true;
    } else {
        QVERIFY(false);
    }

    // If all three are added, this part of the test is over
    if (m_everyonePerson1Added && m_everyonePerson2Added && m_everyonePerson3Added) {
        mLoop->quit();
    }
}

void PeopleManagerTest::everyoneOnPersonRemoved1(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void PeopleManagerTest::everyoneOnPersonAdded2(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_everyonePerson4);

    mLoop->quit();
}

void PeopleManagerTest::everyoneOnPersonRemoved2(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void PeopleManagerTest::everyoneOnPersonAdded3(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void PeopleManagerTest::everyoneOnPersonRemoved3(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_everyonePerson2);

    mLoop->quit();
}

void PeopleManagerTest::cleanup() {
    // Clear the Nepomuk DB
    // TODO: Implement me!
}

void PeopleManagerTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}


QTEST_MAIN(PeopleManagerTest)


#include "people-manager-test.moc"

