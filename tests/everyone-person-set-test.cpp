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

#include "everyone-person-set-test.h"

#include "people-manager.h"

#include "ontologies/pimo.h"

#include <KDebug>

#include <Nepomuk/Thing>

#include <QTest>

using namespace KTelepathy;

EveryonePersonSetTest::EveryonePersonSetTest(QObject* parent)
    : TelepathyBaseTest(parent)
{

}
EveryonePersonSetTest::~EveryonePersonSetTest()
{

}

void EveryonePersonSetTest::initTestCase()
{
    initTestCaseImpl();
}

void EveryonePersonSetTest::init()
{

}

void EveryonePersonSetTest::testAddRemove()
{
    kDebug();

    // Set up member vars used for this test
    m_addRemovePerson1Added = false;
    m_addRemovePerson2Added = false;
    m_addRemovePerson3Added = false;

    // Prepopulate Nepomuk with a load of misc
    m_addRemovePerson1 = QUrl::fromEncoded("nepomuk:/addRemove-test-1");
    m_addRemovePerson2 = QUrl::fromEncoded("nepomuk:/addRemove-test-2");
    m_addRemovePerson3 = QUrl::fromEncoded("nepomuk:/addRemove-test-3");

    Nepomuk::Thing thing1(m_addRemovePerson1);
    QVERIFY(!thing1.exists());
    thing1.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing1.exists());
    QVERIFY(thing1.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    Nepomuk::Thing thing2(m_addRemovePerson2);
    QVERIFY(!thing2.exists());
    thing2.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing2.exists());
    QVERIFY(thing2.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    Nepomuk::Thing thing3(m_addRemovePerson3);
    QVERIFY(!thing3.exists());
    thing3.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing3.exists());
    QVERIFY(thing3.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Get the People Manager instance
    PeopleManager *pm = PeopleManager::instance();
    QVERIFY(pm);

    // Get the PersonSet containing Everyone
    m_addRemovePersonSet = pm->everyone();
    QVERIFY(!m_addRemovePersonSet.isNull());

    // The person set should be empty to begin with
    QCOMPARE(m_addRemovePersonSet->people().size(), 0);

    // Connect to the PersonSet's signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded1(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved1(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 3);

    // Disconnect the signals from part one of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded1(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved1(KTelepathy::PersonPtr)));

    // Next, we add a new person to Nepomuk and see if the set picks it up.
    m_addRemovePerson4 = QUrl::fromEncoded("nepomuk:/addRemove-test-4");
    Nepomuk::Thing thing4(m_addRemovePerson4);
    QVERIFY(!thing4.exists());
    thing4.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(thing4.exists());
    QVERIFY(thing4.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    // Connect to the signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 4);

    // Disconnect the signals from part two of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));

    // Next, remove a person from Nepomuk to see if it picks it up.
    m_addRemovePerson2.remove();

    // Connect to the signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded3(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved3(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 3);

    // Disconnect the signals from part three of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded2(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved2(KTelepathy::PersonPtr)));

    // Cleanup
    m_addRemovePersonSet.clear();

    // Cleanup this test
    m_addRemovePerson1.remove();
    m_addRemovePerson3.remove();
    m_addRemovePerson4.remove();
}

void EveryonePersonSetTest::addRemoveOnPersonAdded1(const PersonPtr &person)
{
    kDebug();

    // See which person was added.
    if (person->resource() == m_addRemovePerson1) {
        QVERIFY(!m_addRemovePerson1Added);
        m_addRemovePerson1Added = true;
    } else if (person->resource() == m_addRemovePerson2) {
        QVERIFY(!m_addRemovePerson2Added);
        m_addRemovePerson2Added = true;
    } else if (person->resource() == m_addRemovePerson3) {
        QVERIFY(!m_addRemovePerson3Added);
        m_addRemovePerson3Added = true;
    } else {
        QVERIFY(false);
    }

    // If all three are added, this part of the test is over
    if (m_addRemovePerson1Added && m_addRemovePerson2Added && m_addRemovePerson3Added) {
        mLoop->quit();
    }
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved1(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::addRemoveOnPersonAdded2(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_addRemovePerson4);

    mLoop->quit();
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved2(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::addRemoveOnPersonAdded3(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved3(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_addRemovePerson2);

    mLoop->quit();
}


void EveryonePersonSetTest::cleanup() {
    // Clear the Nepomuk DB
    // TODO: Implement me!
}

void EveryonePersonSetTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}


QTEST_MAIN(EveryonePersonSetTest)


#include "everyone-person-set-test.moc"

