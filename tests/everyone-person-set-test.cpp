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

#include "ontologies/nco.h"
#include "ontologies/pimo.h"
#include "ontologies/telepathy.h"

#include <qtest_kde.h>

#include <KDebug>

#include <Nepomuk/Thing>
#include <Nepomuk/Variant>

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

    Nepomuk::Resource contact1 = QUrl::fromEncoded("nepomuk:/addRemove-test-contact-1");
    QVERIFY(!contact1.exists());
    contact1.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(contact1.exists());
    QVERIFY(contact1.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    Nepomuk::Resource contact2 = QUrl::fromEncoded("nepomuk:/addRemove-test-contact-2");
    QVERIFY(!contact2.exists());
    contact2.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(contact2.exists());
    QVERIFY(contact2.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    Nepomuk::Resource contact3 = QUrl::fromEncoded("nepomuk:/addRemove-test-contact-3");
    QVERIFY(!contact3.exists());
    contact3.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(contact3.exists());
    QVERIFY(contact3.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    Nepomuk::Resource imaccount1 = QUrl::fromEncoded("nepomuk:/addRemove-test-imaccount-1");
    QVERIFY(!imaccount1.exists());
    imaccount1.addType(Nepomuk::Vocabulary::NCO::IMAccount());
    QVERIFY(imaccount1.exists());
    QVERIFY(imaccount1.hasType(Nepomuk::Vocabulary::NCO::IMAccount()));

    Nepomuk::Resource imaccount2 = QUrl::fromEncoded("nepomuk:/addRemove-test-imaccount-2");
    QVERIFY(!imaccount2.exists());
    imaccount2.addType(Nepomuk::Vocabulary::NCO::IMAccount());
    QVERIFY(imaccount2.exists());
    QVERIFY(imaccount2.hasType(Nepomuk::Vocabulary::NCO::IMAccount()));

    Nepomuk::Resource imaccount3 = QUrl::fromEncoded("nepomuk:/addRemove-test-imaccount-3");
    QVERIFY(!imaccount3.exists());
    imaccount3.addType(Nepomuk::Vocabulary::NCO::IMAccount());
    QVERIFY(imaccount3.exists());
    QVERIFY(imaccount3.hasType(Nepomuk::Vocabulary::NCO::IMAccount()));

    thing1.addGroundingOccurrence(contact1);
    QVERIFY(thing1.groundingOccurrences().contains(contact1));
    QCOMPARE(thing1.groundingOccurrences().length(), 1);

    thing2.addGroundingOccurrence(contact2);
    QVERIFY(thing2.groundingOccurrences().contains(contact2));
    QCOMPARE(thing2.groundingOccurrences().length(), 1);

    thing3.addGroundingOccurrence(contact3);
    QVERIFY(thing3.groundingOccurrences().contains(contact3));
    QCOMPARE(thing3.groundingOccurrences().length(), 1);

    contact1.addProperty(Nepomuk::Vocabulary::NCO::hasIMAccount(), imaccount1);
    QCOMPARE(contact1.properties().count(Nepomuk::Vocabulary::NCO::hasIMAccount()), 1);

    contact2.addProperty(Nepomuk::Vocabulary::NCO::hasIMAccount(), imaccount2);
    QCOMPARE(contact2.properties().count(Nepomuk::Vocabulary::NCO::hasIMAccount()), 1);

    contact3.addProperty(Nepomuk::Vocabulary::NCO::hasIMAccount(), imaccount3);
    QCOMPARE(contact3.properties().count(Nepomuk::Vocabulary::NCO::hasIMAccount()), 1);

    // Create all the "ME" stuff
    Nepomuk::Thing mePerson = QUrl::fromEncoded("nepomuk:/myself");
    QVERIFY(!mePerson.exists());
    mePerson.addType(Nepomuk::Vocabulary::PIMO::Person());
    QVERIFY(mePerson.exists());
    QVERIFY(mePerson.hasType(Nepomuk::Vocabulary::PIMO::Person()));

    Nepomuk::Resource mePersonContact = QUrl::fromEncoded("nepomuk:/myself-person-contact");
    QVERIFY(!mePersonContact.exists());
    mePersonContact.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(mePersonContact.exists());
    QVERIFY(mePersonContact.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    Nepomuk::Resource meIMAccount = QUrl::fromEncoded("nepomuk:/myself-im-account-1");
    QVERIFY(!meIMAccount.exists());
    meIMAccount.addType(Nepomuk::Vocabulary::NCO::IMAccount());
    QVERIFY(meIMAccount.exists());
    QVERIFY(meIMAccount.hasType(Nepomuk::Vocabulary::NCO::IMAccount()));

    mePerson.addGroundingOccurrence(mePersonContact);
    QVERIFY(mePerson.groundingOccurrences().contains(mePersonContact));
    QCOMPARE(mePerson.groundingOccurrences().length(), 1);

    mePersonContact.addProperty(Nepomuk::Vocabulary::NCO::hasIMAccount(), meIMAccount);
    QCOMPARE(mePersonContact.properties().count(Nepomuk::Vocabulary::NCO::hasIMAccount()), 1);

    // Create all the relationships between people etc
    imaccount1.addProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf(), meIMAccount);
    imaccount1.addProperty(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo(), meIMAccount);
    QCOMPARE(imaccount1.properties().count(Nepomuk::Vocabulary::Telepathy::isBuddyOf()), 1);
    QCOMPARE(imaccount1.properties().count(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo()), 1);

    imaccount2.addProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf(), meIMAccount);
    imaccount2.addProperty(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo(), meIMAccount);
    QCOMPARE(imaccount2.properties().count(Nepomuk::Vocabulary::Telepathy::isBuddyOf()), 1);
    QCOMPARE(imaccount2.properties().count(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo()), 1);

    imaccount3.addProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf(), meIMAccount);
    imaccount3.addProperty(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo(), meIMAccount);
    QCOMPARE(imaccount3.properties().count(Nepomuk::Vocabulary::Telepathy::isBuddyOf()), 1);
    QCOMPARE(imaccount3.properties().count(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo()), 1);

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

    Nepomuk::Resource contact4 = QUrl::fromEncoded("nepomuk:/addRemove-test-contact-4");
    QVERIFY(!contact4.exists());
    contact4.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(contact4.exists());
    QVERIFY(contact4.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    Nepomuk::Resource imaccount4 = QUrl::fromEncoded("nepomuk:/addRemove-test-imaccount-4");
    QVERIFY(!imaccount4.exists());
    imaccount4.addType(Nepomuk::Vocabulary::NCO::IMAccount());
    QVERIFY(imaccount4.exists());
    QVERIFY(imaccount4.hasType(Nepomuk::Vocabulary::NCO::IMAccount()));

    thing4.addGroundingOccurrence(contact4);
    QVERIFY(thing4.groundingOccurrences().contains(contact4));
    QCOMPARE(thing4.groundingOccurrences().length(), 1);

    contact4.addProperty(Nepomuk::Vocabulary::NCO::hasIMAccount(), imaccount4);
    QCOMPARE(contact4.properties().count(Nepomuk::Vocabulary::NCO::hasIMAccount()), 1);

    imaccount4.addProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf(), meIMAccount);
    imaccount4.addProperty(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo(), meIMAccount);
    QCOMPARE(imaccount4.properties().count(Nepomuk::Vocabulary::Telepathy::isBuddyOf()), 1);
    QCOMPARE(imaccount4.properties().count(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo()), 1);

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
    QVERIFY(!m_addRemovePerson2.exists());

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
               SLOT(addRemoveOnPersonAdded3(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved3(KTelepathy::PersonPtr)));

    // Break the isBuddyOf relationship
    //imaccount1.removeProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf(), meIMAccount);
    imaccount1.removeProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf());
    QCOMPARE(imaccount1.properties().count(Nepomuk::Vocabulary::Telepathy::isBuddyOf()), 0);

    // Connect to the signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded4(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved4(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 2);

    // Disconnect the signals from part three of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded4(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved4(KTelepathy::PersonPtr)));

    // Break the publishesPresenceTo relationship
    imaccount4.removeProperty(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo(), meIMAccount);
    QCOMPARE(imaccount4.properties().count(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo()), 0);

    // Connect to the signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded5(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved5(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 1);

    // Disconnect the signals from part three of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded5(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved5(KTelepathy::PersonPtr)));

    // Re-add the isBuddyOf relationship
    imaccount1.addProperty(Nepomuk::Vocabulary::Telepathy::isBuddyOf(), meIMAccount);
    QCOMPARE(imaccount1.properties().count(Nepomuk::Vocabulary::Telepathy::isBuddyOf()), 1);

    // Connect to the signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded6(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved6(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 2);

    // Disconnect the signals from part three of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded6(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved6(KTelepathy::PersonPtr)));

    // Re-add the publishesPresenceTo relationship
    imaccount4.addProperty(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo(), meIMAccount);
    QCOMPARE(imaccount4.properties().count(Nepomuk::Vocabulary::Telepathy::publishesPresenceTo()), 1);

    // Connect to the signals
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personAdded(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonAdded7(KTelepathy::PersonPtr)));
    connect(m_addRemovePersonSet.data(),
            SIGNAL(personRemoved(KTelepathy::PersonPtr)),
            SLOT(addRemoveOnPersonRemoved7(KTelepathy::PersonPtr)));

    // Launch the mainloop
    mLoop->exec();

    QCOMPARE(m_addRemovePersonSet->people().size(), 3);

    // Disconnect the signals from part three of the test.
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personAdded(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonAdded7(KTelepathy::PersonPtr)));
    disconnect(m_addRemovePersonSet.data(),
               SIGNAL(personRemoved(KTelepathy::PersonPtr)),
               this,
               SLOT(addRemoveOnPersonRemoved7(KTelepathy::PersonPtr)));

    // Cleanup
    m_addRemovePersonSet.clear();

    // TODO: Cleanup properly

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

    QCOMPARE(person->resourceUri(), QUrl::fromEncoded("nepomuk:/addRemove-test-2"));

    mLoop->quit();
}

void EveryonePersonSetTest::addRemoveOnPersonAdded4(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved4(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_addRemovePerson1);

    mLoop->quit();
}

void EveryonePersonSetTest::addRemoveOnPersonAdded5(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved5(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_addRemovePerson4);

    mLoop->quit();
}

void EveryonePersonSetTest::addRemoveOnPersonAdded6(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_addRemovePerson1);

    mLoop->quit();
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved6(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::addRemoveOnPersonAdded7(const PersonPtr &person)
{
    kDebug();

    QCOMPARE(person->resource(), m_addRemovePerson4);

    mLoop->quit();
}

void EveryonePersonSetTest::addRemoveOnPersonRemoved7(const PersonPtr &person)
{
    kDebug();

    Q_UNUSED(person);

    // This *should* never be called
    QVERIFY(false);
}

void EveryonePersonSetTest::cleanup() {
    // Clear the Nepomuk DB
    // TODO: Implement me!
}

void EveryonePersonSetTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}

//QTEST_MAIN(EveryonePersonSet)
QTEST_KDEMAIN(EveryonePersonSetTest, GUI)


#include "everyone-person-set-test.moc"

