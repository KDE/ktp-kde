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

using namespace KTelepathy;

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
    // Get the People Manager instance
    PeopleManager *pm = PeopleManager::instance();
    QVERIFY(pm);

    // Get the PersonSet containing Everyone
    PersonSetPtr everyonePersonSet = pm->everyone();
    QVERIFY(!everyonePersonSet.isNull());

    // Check that the EveryonePersonSet is cached properly.
    PersonSetPtr personSet = pm->everyone();
    QCOMPARE(personSet.data(), everyonePersonSet.data());

    // Check the cache gets cleared properly
    QWeakPointer<PersonSet> weakPtr = everyonePersonSet.toWeakRef();
    everyonePersonSet.clear();
    personSet.clear();

    PersonSetPtr newPersonSet = pm->everyone();
    QVERIFY(!newPersonSet.isNull());
    QVERIFY(weakPtr.isNull());
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

