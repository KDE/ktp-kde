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

#ifndef PERSONSET_TEST_H
#define PERSONSET_TEST_H

#include "person-set.h"

#include <KTelepathy/TestLib/NepomukTest>

#include <Nepomuk/Resource>

class PersonSetTest : public NepomukTest
{
    Q_OBJECT

public:
    PersonSetTest(QObject* parent = 0);
    virtual ~PersonSetTest();

protected Q_SLOTS:
    // testAddRemove()
    void addRemoveOnPersonAdded1(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonAdded2(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonRemoved1(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonRemoved2(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonRemoved3(const KTelepathy::PersonPtr &person);
    void addRemoveOnPersonRemoved4(const KTelepathy::PersonPtr &person);

private Q_SLOTS:
    void initTestCase();
    void init();

    void testConstructorDestructorCreate();
    void testAddRemove();
    void testPeople();

    void cleanup();
    void cleanupTestCase();

private:
    // testAddRemove()
    KTelepathy::PersonSetPtr m_addRemovePersonSet;
    KTelepathy::PersonPtr m_addRemovePerson1;
    KTelepathy::PersonPtr m_addRemovePerson2;
    Nepomuk::Resource m_addRemovePersonResource1;
    Nepomuk::Resource m_addRemovePersonResource2;
    bool m_addRemoveSlotCalled;

};


#endif // PERSONSET_TEST_H

