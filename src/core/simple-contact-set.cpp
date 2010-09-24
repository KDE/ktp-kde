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

#include "simple-contact-set.h"

#include <KDebug>

using namespace KTelepathy;

class SimpleContactSet::Private {

public:
    Private()
    { }
};

SimpleContactSetPtr SimpleContactSet::create()
{
    return SimpleContactSetPtr(new SimpleContactSet);
}

SimpleContactSet::SimpleContactSet()
: ContactSet(),
d(new SimpleContactSet::Private)
{
    kDebug();
}

SimpleContactSet::~SimpleContactSet()
{
    kDebug();

    delete d;
}

void SimpleContactSet::addContact(const ContactPtr &contact)
{
    ContactSet::addContact(contact);
}

void SimpleContactSet::removePerson(const ContactPtr &contact)
{
    ContactSet::removeContact(contact);
}

void SimpleContactSet::removeContact(const QUrl &uri)
{
    ContactSet::removeContact(uri);
}


#include "simple-contact-set.moc"

