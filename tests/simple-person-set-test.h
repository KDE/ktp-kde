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

#ifndef LIBKTELEPATHY_TESTS_SIMPLE_PERSON_SET_TEST_H
#define LIBKTELEPATHY_TESTS_SIMPLE_PERSON_SET_TEST_H

#include "simple-person-set.h"

#include <KTelepathy/TestLib/NepomukTest>

#include <QtCore/QUrl>

class SimplePersonSetTest : public NepomukTest
{
    Q_OBJECT

public:
    SimplePersonSetTest(QObject* parent = 0);
    virtual ~SimplePersonSetTest();

protected Q_SLOTS:
    // testAddRemove()
    void addRemoveOnPersonAdded1(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonAdded2(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonRemoved1(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonRemoved2(const KTelepathy::PersonPtr &person);

private Q_SLOTS:
    void initTestCase();
    void init();

    void testConstructorDestructorCreate();
    void testAddRemove();

    void cleanup();
    void cleanupTestCase();

private:
    // testAddRemove()
    KTelepathy::SimplePersonSetPtr m_addRemovePersonSet;
    KTelepathy::PersonPtr m_addRemovePerson1;
    KTelepathy::PersonPtr m_addRemovePerson2;
    QUrl m_addRemovePersonUri1;
    QUrl m_addRemovePersonUri2;
    bool m_addRemoveSlotCalled;

};


#endif // LIBKTELEPATHY_TESTS_SIMPLE_PERSON_SET_TEST_H

