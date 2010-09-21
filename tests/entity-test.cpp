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

void EntityTest::testOperatorsEqualsNotEqual()
{
    // Try equating two invalid Entities.

    // Try equating two different valid entities

    // Try equating two same valid entities

    // Try equating two same valid, but removed from nepomuk entities

    // Try equating two different valid, but removed from nepomuk entities.
}

void EntityTest::testqHash()
{

}

void EntityTest::testResource()
{

}

void EntityTest::testResourceUri()
{

}

void EntityTest::testValidSetValid()
{

}


QTEST_KDEMAIN(EntityTest, GUI)


#include "entity-test.moc"

