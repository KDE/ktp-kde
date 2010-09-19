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


QTEST_KDEMAIN(EntityTest, GUI)


#include "entity-test.moc"

