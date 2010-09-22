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

#include "simple-person-set-test.h"

#include "fake-person.h"

#include "test-backdoors.h"

#include "ontologies/pimo.h"

#include <qtest_kde.h>
#include <KDebug>

#include <QTest>

using namespace KTelepathy;

SimplePersonSetTest::SimplePersonSetTest(QObject* parent)
: Tp::NepomukTest(parent)
{

}
SimplePersonSetTest::~SimplePersonSetTest()
{

}

void SimplePersonSetTest::initTestCase()
{
    initTestCaseImpl();
}

void SimplePersonSetTest::init()
{
    initImpl();
}

void SimplePersonSetTest::testConstructorDestructorCreate()
{
    // Create a PersonSet.
    SimplePersonSetPtr personSet = SimplePersonSet::create();
    QVERIFY(!personSet.isNull());

    // Get a QWeakPointer to it, for testing destruction.
    QWeakPointer<PersonSet> weakPtr = personSet.toWeakRef();

    // Remove the only strong ref.
    personSet.clear();

    // Check the PersonSet was deleted OK
    QVERIFY(personSet.isNull());
    QVERIFY(weakPtr.isNull());
}

void SimplePersonSetTest::testAddRemove()
{
    // First, create the PersonSet.
    m_addRemovePersonSet = SimplePersonSet::create();
    QVERIFY(!m_addRemovePersonSet.isNull());
    QCOMPARE(m_addRemovePersonSet->people().size(), 0);

    // Create a couple of Fake Persons
    m_addRemovePersonUri1 = QUrl::fromEncoded("nepomuk:/p1");
    m_addRemovePersonUri2 = QUrl::fromEncoded("nepomuk:/p2");
    m_addRemovePerson1 = QSharedPointer<Person>(new FakePerson(m_addRemovePersonUri1));
    m_addRemovePerson2 = QSharedPointer<Person>(new FakePerson(m_addRemovePersonUri2));

    // Add them (one at a time) to the person set.
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonAdded1(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    m_addRemovePersonSet->addPerson(m_addRemovePerson1);

    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded1(KTelepathy::PersonPtr)));

    QCOMPARE(m_addRemovePersonSet->people().size(), 1);
    QVERIFY(m_addRemoveSlotCalled);

    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    m_addRemovePersonSet->addPerson(m_addRemovePerson2);

    QCOMPARE(m_addRemovePersonSet->people().size(), 2);
    QVERIFY(m_addRemoveSlotCalled);

    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));

    // Remove a person by PersonPtr
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonRemoved1(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    m_addRemovePersonSet->removePerson(m_addRemovePerson1);

    QCOMPARE(m_addRemovePersonSet->people().size(), 1);
    QVERIFY(m_addRemoveSlotCalled);

    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved1(KTelepathy::PersonPtr)));

    // Remove a person by URI
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            this,
            SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));
    m_addRemoveSlotCalled = false;

    m_addRemovePersonSet->removePerson(m_addRemovePersonUri2);

    QCOMPARE(m_addRemovePersonSet->people().size(), 0);
    QVERIFY(m_addRemoveSlotCalled);

    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));
}

void SimplePersonSetTest::addRemoveOnPersonAdded1(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson1);

    m_addRemoveSlotCalled = true;
}

void SimplePersonSetTest::addRemoveOnPersonAdded2(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson2);

    m_addRemoveSlotCalled = true;
}


void SimplePersonSetTest::addRemoveOnPersonRemoved1(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson1);

    m_addRemoveSlotCalled = true;
}

void SimplePersonSetTest::addRemoveOnPersonRemoved2(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person, m_addRemovePerson2);

    m_addRemoveSlotCalled = true;
}

void SimplePersonSetTest::cleanup()
{
    cleanupImpl();
}

void SimplePersonSetTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}


QTEST_KDEMAIN(SimplePersonSetTest, GUI)


#include "simple-person-set-test.moc"

