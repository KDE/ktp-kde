/*
 * This file is part of libktelepathy
 *
 * Copyright (C) 2011 Francesco Nwokeka <francesco.nwokeka@gmail.com>
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

#ifndef LIBKTELEPATHY_EVERYONE_PERSON_SET_MODEL_H
#define LIBKTELEPATHY_EVERYONE_PERSON_SET_MODEL_H

#include "person-set-model.h"

/**
 * @class EveryonePersonSetModel
 *
 * @brief Wrapper class for PersonSetModel
 *
 * This class is used as a wrapper class for the PersonSetModel class. It loads all
 * contact info upon creation and is used by the QML plugin to access contact data
 * @author Francesco Nwokeka
 */

namespace KTelepathy {

class KDE_EXPORT EveryonePersonSetModel: public PersonSetModel
{
public:
    explicit EveryonePersonSetModel(QObject* parent = 0);
    virtual ~EveryonePersonSetModel();
};

}

#endif  // LIBKTELEPATHY_EVERYONE_PERSON_SET_MODEL_H
