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

#include "person-set-model-test.h"

#include "fake-person.h"
#include "fake-person-set.h"

#include <KDebug>

#include <QtCore/QTimer>

#include <QTest>

using namespace KTelepathy;

PersonSetModelTest::PersonSetModelTest(QObject* parent)
    : Tp::NepomukTest(parent)
{

}
PersonSetModelTest::~PersonSetModelTest()
{

}

void PersonSetModelTest::initTestCase()
{
    initTestCaseImpl();
}

void PersonSetModelTest::init()
{
    initImpl();
}

void PersonSetModelTest::testConstructorDestructor()
{
    // Construct a model without a PersonSet
    PersonSetModel *model = new PersonSetModel();
    ModelTest *modelTest = new ModelTest(model);
    QVERIFY(model);
    QVERIFY(modelTest);
    QVERIFY(!model->personSet().data());

    // Destroy it
    model->deleteLater();
    connect(model, SIGNAL(destroyed()), SLOT(constructorDestructorOnModelDeleted()));
    mLoop->exec();

    modelTest->deleteLater();
    connect(modelTest, SIGNAL(destroyed()), SLOT(constructorDestructorOnModelTestDeleted()));
    mLoop->exec();

    // Construct a model with a PersonSet
    QSharedPointer<FakePersonSet> fakePersonSet(new FakePersonSet);
    QVERIFY(fakePersonSet.data());
    QSharedPointer<FakePerson> fakePerson1(new FakePerson(QUrl::fromEncoded("nepomuk:/test1")));
    QVERIFY(fakePerson1.data());
    QCOMPARE(fakePerson1->resourceUri(), QUrl::fromEncoded("nepomuk:/test1"));
    QSharedPointer<FakePerson> fakePerson2(new FakePerson(QUrl::fromEncoded("nepomuk:/test2")));
    QVERIFY(fakePerson2.data());
    QCOMPARE(fakePerson2->resourceUri(), QUrl::fromEncoded("nepomuk:/test2"));
    QSharedPointer<FakePerson> fakePerson3(new FakePerson(QUrl::fromEncoded("nepomuk:/test3")));
    QVERIFY(fakePerson3.data());
    QCOMPARE(fakePerson3->resourceUri(), QUrl::fromEncoded("nepomuk:/test3"));

    fakePersonSet->addPerson(fakePerson1);
    fakePersonSet->addPerson(fakePerson2);
    fakePersonSet->addPerson(fakePerson3);

    QCOMPARE(fakePersonSet->people().size(), 3);

    model = new PersonSetModel(fakePersonSet);
    modelTest = new ModelTest(model);
    QVERIFY(model);
    QVERIFY(modelTest);
    QCOMPARE(model->personSet().data(), fakePersonSet.data());

    // Destroy it
    model->deleteLater();
    connect(model, SIGNAL(destroyed()), SLOT(constructorDestructorOnModelDeleted()));
    mLoop->exec();

    modelTest->deleteLater();
    connect(modelTest, SIGNAL(destroyed()), SLOT(constructorDestructorOnModelTestDeleted()));
    mLoop->exec();
}

void PersonSetModelTest::constructorDestructorOnModelDeleted()
{
    mLoop->quit();
}

void PersonSetModelTest::constructorDestructorOnModelTestDeleted()
{
    mLoop->quit();
}

void PersonSetModelTest::cleanup() {
    // Clear the Nepomuk DB
    // TODO: Implement me!
    cleanupImpl();
}

void PersonSetModelTest::cleanupTestCase()
{
    cleanupTestCaseImpl();
}


QTEST_MAIN(PersonSetModelTest)


#include "person-set-model-test.moc"

