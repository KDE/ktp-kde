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

#include "entity-test.h"

#include "entity.h"
#include "test-backdoors.h"

#include "ontologies/nco.h"

#include <qtest_kde.h>

#include <KDebug>

#include <QTest>

using namespace KTelepathy;

EntityTest::EntityTest(QObject* parent)
: Tp::NepomukTest(parent)
{

}
EntityTest::~EntityTest()
{

}

void EntityTest::initTestCase()
{
    initTestCaseImpl();
}

void EntityTest::init()
{
    initImpl();
}

void EntityTest::cleanup()
{
    cleanupImpl();
}

void EntityTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}

void EntityTest::testConstructorDestructor()
{
    // Test the invalid constructor
    EntityPtr entity1 = TestBackdoors::entityConstruct();
    QVERIFY(!entity1.isNull());

    QWeakPointer<Entity> weakPtr1 = entity1.toWeakRef();
    QVERIFY(!weakPtr1.isNull());

    entity1.clear();
    QVERIFY(entity1.isNull());
    QVERIFY(weakPtr1.isNull());

    // Test the constructor that takes a Nepomuk Resource.
    Nepomuk::Resource resource(QUrl::fromEncoded("nepomuk:/entity-test-constructor"));
    resource.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(resource.exists());
    QVERIFY(resource.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    EntityPtr entity2 = TestBackdoors::entityConstruct(resource);
    QVERIFY(!entity2.isNull());

    QWeakPointer<Entity> weakPtr2 = entity2.toWeakRef();
    QVERIFY(!weakPtr2.isNull());

    entity2.clear();
    QVERIFY(entity2.isNull());
    QVERIFY(weakPtr2.isNull());

    // Cleanup
    resource.remove();
}

void EntityTest::testqHash()
{
    // Use a QSet to test the qHash implementation
    QSet<EntityPtr> entities;

    // Add a valid Entity Ptr
    Nepomuk::Resource resource1(QUrl::fromEncoded("nepomuk:/entity-test-qHash-1"));
    resource1.addType(Nepomuk::Vocabulary::NCO::PersonContact());

    QVERIFY(resource1.exists());
    QVERIFY(resource1.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    EntityPtr entity1 = TestBackdoors::entityConstruct(resource1);

    entities.insert(entity1);

    QCOMPARE(entities.size(), 1);
    QVERIFY(entities.contains(entity1));

    // Add another valid Entity Ptr
    Nepomuk::Resource resource2(QUrl::fromEncoded("nepomuk:/entity-test-qHash-2"));
    resource2.addType(Nepomuk::Vocabulary::NCO::PersonContact());

    QVERIFY(resource2.exists());
    QVERIFY(resource2.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    EntityPtr entity2 = TestBackdoors::entityConstruct(resource2);

    entities.insert(entity2);

    QCOMPARE(entities.size(), 2);
    QVERIFY(entities.contains(entity1));
    QVERIFY(entities.contains(entity2));

    // Add the same EntityPtr again
    entities.insert(entity2);

    QCOMPARE(entities.size(), 2);
    QVERIFY(entities.contains(entity1));
    QVERIFY(entities.contains(entity2));

    // Add a different EntityPtr to the same Entity again.
    EntityPtr entity3 = TestBackdoors::entityConstruct(resource2);

    entities.insert(entity3);

    QCOMPARE(entities.size(), 3);
    QVERIFY(entities.contains(entity1));
    QVERIFY(entities.contains(entity2));
    QVERIFY(entities.contains(entity3));

    // Add an invalid EntityPtr
    EntityPtr entity4 = TestBackdoors::entityConstruct();

    entities.insert(entity4);

    QCOMPARE(entities.size(), 4);
    QVERIFY(entities.contains(entity1));
    QVERIFY(entities.contains(entity2));
    QVERIFY(entities.contains(entity3));
    QVERIFY(entities.contains(entity4));

    // Add the same invalid EntityPtr again
    entities.insert(entity4);

    QCOMPARE(entities.size(), 4);
    QVERIFY(entities.contains(entity1));
    QVERIFY(entities.contains(entity2));
    QVERIFY(entities.contains(entity3));
    QVERIFY(entities.contains(entity4));

    // Add another invalid EntityPtr
    EntityPtr entity5 = TestBackdoors::entityConstruct();

    entities.insert(entity5);

    QCOMPARE(entities.size(), 5);
    QVERIFY(entities.contains(entity1));
    QVERIFY(entities.contains(entity2));
    QVERIFY(entities.contains(entity3));
    QVERIFY(entities.contains(entity4));
    QVERIFY(entities.contains(entity5));

    // Cleanup Nepomuk data
    resource1.remove();
    resource2.remove();
}

void EntityTest::testResource()
{
    // Create a valid Entity Ptr
    Nepomuk::Resource resource(QUrl::fromEncoded("nepomuk:/entity-test-resource-1"));
    resource.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(resource.exists());
    QVERIFY(resource.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    EntityPtr entity1 = TestBackdoors::entityConstruct(resource);

    // The resource should be invalid until we setValid the entity.
    QCOMPARE(entity1->resource(), Nepomuk::Resource());

    // But valid, once setValid is true
    TestBackdoors::entitySetValid(entity1, true);
    QCOMPARE(entity1->resource(), resource);

    // Remove the resource from Nepomuk
    resource.remove();

    // Check the resource() method is now invalid
    QCOMPARE(entity1->resource(), Nepomuk::Resource());

    // An invalid entity should return an invalid resource
    EntityPtr entity2 = TestBackdoors::entityConstruct();
    QCOMPARE(entity2->resource(), Nepomuk::Resource());

    // Cleanup Nepomuk data
    // No cleanup needed here
}

void EntityTest::testResourceUri()
{
    // Create an invalid Entity Ptr
    EntityPtr entity1 = TestBackdoors::entityConstruct();

    // Check the resourceUri() is invalid
    QCOMPARE(entity1->resourceUri(), QUrl());

    // Create a valid Entity Ptr
    QUrl resourceUri = QUrl::fromEncoded("nepomuk:/entity-test-resource-uri-1");
    Nepomuk::Resource resource(resourceUri);
    resource.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(resource.exists());
    QVERIFY(resource.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    EntityPtr entity2 = TestBackdoors::entityConstruct(resource);

    // The resourceUri should be valid immediately
    QCOMPARE(entity2->resourceUri(), resourceUri);

    // Remove the resource from Nepomuk
    resource.remove();

    // Check the resource URI is still valid
    QCOMPARE(entity2->resourceUri(), resourceUri);

    // Cleanup Nepomuk data
    // No cleanup needed here
}

void EntityTest::testValidSetValid()
{
    // Create an entity from a valid Nepomuk Resource
    Nepomuk::Resource resource(QUrl::fromEncoded("nepomuk:/entity-test-valid-set-valid-1"));
    resource.addType(Nepomuk::Vocabulary::NCO::PersonContact());
    QVERIFY(resource.exists());
    QVERIFY(resource.hasType(Nepomuk::Vocabulary::NCO::PersonContact()));

    EntityPtr entity1 = TestBackdoors::entityConstruct(resource);

    // Check that it is invalid
    QVERIFY(!entity1->isValid());

    // Mark it as valid
    TestBackdoors::entitySetValid(entity1, true);
    QVERIFY(entity1->isValid());

    // Remove the Nepomuk Resource and check it remains valid
    resource.remove();
    QVERIFY(entity1->isValid());

    // Manually set it as invalid, as a subclass would
    TestBackdoors::entitySetValid(entity1, false);
    QVERIFY(!entity1->isValid());

    // Check that a newly constructed invalid entity is valid() == false
    EntityPtr entity2 = TestBackdoors::entityConstruct();
    QVERIFY(!entity2->isValid());

    // Cleanup Nepomuk data
    // Nothing to cleanup here
}


QTEST_KDEMAIN(EntityTest, GUI)


#include "entity-test.moc"

