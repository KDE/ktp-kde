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

#ifndef PERSONSETMODEL_TEST_H
#define PERSONSETMODEL_TEST_H

#include "person-set-model.h"

#include <KTelepathy/TestLib/telepathy-base-test.h>

#include "modeltest.h"

class PersonSetModelTest : public TelepathyBaseTest
{
    Q_OBJECT

public:
    PersonSetModelTest(QObject* parent = 0);
    virtual ~PersonSetModelTest();

protected Q_SLOTS:
    // testConstructorDestructor()
    void constructorDestructorOnModelDeleted();
    void constructorDestructorOnModelTestDeleted();

private Q_SLOTS:
    void initTestCase();
    void init();

    void testConstructorDestructor();

    void cleanup();
    void cleanupTestCase();

private:
    

};


#endif // PERSONSETMODEL_TEST_H

