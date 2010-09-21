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

#ifndef LIBKTELEPATHY_TEST_BACKDOORS_H
#define LIBKTELEPATHY_TEST_BACKDOORS_H

#include "entity.h"
#include "person-set.h"

#include <kdemacros.h>

namespace KTelepathy {

class KDE_EXPORT TestBackdoors {

public:
    static PersonSetPtr createPersonSet();
    static void personSetAddPerson(PersonSetPtr personSet, PersonPtr person);
    static void personSetRemovePerson(PersonSetPtr personSet, PersonPtr person);
    static void personSetRemovePerson(PersonSetPtr personSet, const QUrl &url);
    static void entitySetValid(EntityPtr entity, bool valid);
    static EntityPtr entityConstruct(const Nepomuk::Resource &resource);
    static EntityPtr entityConstruct();
};

}  // namespace KTelepathy


#endif  // Header guard

