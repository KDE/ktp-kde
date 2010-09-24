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

#include "person-set-test.h"

#include "people-manager.h"
#include "test-backdoors.h"

#include "ontologies/pimo.h"

#include <qtest_kde.h>
#include <KDebug>

#include <Nepomuk/Thing>

#include <QtCore/QTimer>

#include <QTest>

using namespace KTelepathy;

PersonSetTest::PersonSetTest(QObject* parent)
    : Tp::NepomukTest(parent)
{

}
PersonSetTest::~PersonSetTest()
{

}

void PersonSetTest::initTestCase()
{
    initTestCaseImpl();
}

void PersonSetTest::init()
{
    initImpl();
}

void PersonSetTest::testConstructorDestructorCreate()
{
    // Create a PersonSet.
    PersonSetPtr personSet = TestBackdoors::personSetCreate();
    QVERIFY(!personSet.isNull());

    // Get a QWeakPointer to it, for testing destruction.
    Tp::WeakPtr<PersonSet> weakPtr = Tp::WeakPtr<PersonSet>(personSet);

    // Remove the only strong ref.
    personSet.reset();

    // Check the PersonSet was deleted OK
    QVERIFY(personSet.isNull());
    QVERIFY(weakPtr.isNull());
}

void PersonSetTest::testAddRemove()
{
    // First, create the PersonSet.
    m_addRemovePersonSet = TestBackdoors::personSetCreate();
    QVERIFY(!m_addRemovePersonSet.isNull());
    QCOMPARE(m_addRemovePersonSet->people().size(), 0);

    // Now we create a single Nepomuk Person resource and a PersonPtr
    m_addRemovePersonResource1 = QUrl::fromEncoded("nepomuk:/add-remove-test-person-1");
    Nepomuk::Thing personThing1(m_addRemovePersonResource1);
    QVERIFY(!personThing1.exists());
    personThing1.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(personThing1.exists());
    QVERIFY(personThing1.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    m_addRemovePerson1 = PeopleManager::instance()->personForResource(m_addRemovePersonResource1);
    QVERIFY(!m_addRemovePerson1.isNull());

    // Connect to the added signal
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonAdded1(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    // Add the person to the set
    TestBackdoors::personSetAddPerson(m_addRemovePersonSet, m_addRemovePerson1);
    QCOMPARE(m_addRemovePersonSet->people().size(), 1);

    QVERIFY(m_addRemoveSlotCalled);

    // Cleanup
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded1(KTelepathy::PersonPtr)));

    // Next, test adding the same resource again.
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    TestBackdoors::personSetAddPerson(m_addRemovePersonSet, m_addRemovePerson1);
    QCOMPARE(m_addRemovePersonSet->people().size(), 1);

    // Check the slot wasn't called.
    QVERIFY(!m_addRemoveSlotCalled);

    // Cleanup
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));

    // Next, add another person to the set, since we will have two variations of the remove()
    // method to test.
    m_addRemovePersonResource2 = QUrl::fromEncoded("nepomuk:/add-remove-test-person-2");
    Nepomuk::Thing personThing2(m_addRemovePersonResource2);
    QVERIFY(!personThing2.exists());
    personThing2.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(personThing2.exists());
    QVERIFY(personThing2.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    m_addRemovePerson2 = PeopleManager::instance()->personForResource(m_addRemovePersonResource2);
    QVERIFY(!m_addRemovePerson2.isNull());

    TestBackdoors::personSetAddPerson(m_addRemovePersonSet, m_addRemovePerson2);
    QCOMPARE(m_addRemovePersonSet->people().size(), 2);

    // Connect to the remove signal
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonRemoved1(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    // Remove a Person by Person object
    TestBackdoors::personSetRemovePerson(m_addRemovePersonSet, m_addRemovePerson1);
    QCOMPARE(m_addRemovePersonSet->people().size(), 1);

    // Check the slot was called.
    QVERIFY(m_addRemoveSlotCalled);

    // Cleanup
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved1(KTelepathy::PersonPtr)));


    // Remove the same one again to check no signal is emitted
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    TestBackdoors::personSetRemovePerson(m_addRemovePersonSet, m_addRemovePerson1);
    QCOMPARE(m_addRemovePersonSet->people().size(), 1);

    // Check the slot wasn't called
    QVERIFY(!m_addRemoveSlotCalled);

    // Cleanup
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));

    // Remove a Person by URI
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonRemoved3(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    TestBackdoors::personSetRemovePerson(m_addRemovePersonSet, m_addRemovePerson2->resourceUri());
    QCOMPARE(m_addRemovePersonSet->people().size(), 0);

    // See if the slot was called.
    QVERIFY(m_addRemoveSlotCalled);

    // Cleanup
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved3(KTelepathy::PersonPtr)));

    // Remove the same one again to check no signal is emitted
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonRemoved4(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    TestBackdoors::personSetRemovePerson(m_addRemovePersonSet, m_addRemovePerson2->resourceUri());
    QCOMPARE(m_addRemovePersonSet->people().size(), 0);

    // Check the slot wasn't called.
    QVERIFY(!m_addRemoveSlotCalled);

    // Cleanup
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved4(KTelepathy::PersonPtr)));

    // Cleanup test
    m_addRemovePersonResource1.remove();
    m_addRemovePersonResource2.remove();
}

void PersonSetTest::addRemoveOnPersonAdded1(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson1);

    m_addRemoveSlotCalled = true;
}

void PersonSetTest::addRemoveOnPersonAdded2(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    m_addRemoveSlotCalled = true;
}

void PersonSetTest::addRemoveOnPersonRemoved1(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson1);

    m_addRemoveSlotCalled = true;
}

void PersonSetTest::addRemoveOnPersonRemoved2(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    m_addRemoveSlotCalled = true;
}

void PersonSetTest::addRemoveOnPersonRemoved3(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson2);

    m_addRemoveSlotCalled = true;
}

void PersonSetTest::addRemoveOnPersonRemoved4(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    m_addRemoveSlotCalled = true;
}

void PersonSetTest::testPeople()
{
    // Create a person set.
    PersonSetPtr personSet = TestBackdoors::personSetCreate();
    QVERIFY(personSet);
    QVERIFY(personSet->people().isEmpty());

    // Create a couple of PIMO:Person's
    Nepomuk::Thing person1(QUrl::fromEncoded("nepomuk:/test-people-person-1"));
    Nepomuk::Thing person2(QUrl::fromEncoded("nepomuk:/test-people-person-2"));
    person1.addType(Nepomuk::Vocabulary::PIMO::Person());
    person2.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(person1.exists());
    QVERIFY(person2.exists());
    QVERIFY(person1.hasType(Nepomuk::Vocabulary::PIMO::Person()));
    QVERIFY(person2.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Add the PIMO:Person's to the PersonSet.
    PeopleManager *pm = PeopleManager::instance();
    PersonPtr personPtr1 = pm->personForResource(person1);
    PersonPtr personPtr2 = pm->personForResource(person2);
    QVERIFY(personPtr1.data() != personPtr2.data());
    TestBackdoors::personSetAddPerson(personSet, personPtr1);
    TestBackdoors::personSetAddPerson(personSet, personPtr2);

    // Check that both people are in PersonSet
    QCOMPARE(personSet->people().size(), 2);
    QVERIFY(personSet->people().contains(personPtr1));
    QVERIFY(personSet->people().contains(personPtr2));

    // Cleanup: remove created Nepomuk resources
    person1.remove();
    person2.remove();
}

void PersonSetTest::cleanup()
{
    cleanupImpl();
}

void PersonSetTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}


QTEST_KDEMAIN(PersonSetTest, GUI)


#include "person-set-test.moc"

