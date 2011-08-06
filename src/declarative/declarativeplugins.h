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

#ifndef LIBKTELEPATHY_DECLARATIVEPLUGINS_H
#define LIBKTELEPATHY_DECLARATIVEPLUGINS_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

/**
 * @class DeclarativePlugins
 *
 * @brief Class to enable access via QML
 *
 * This class permits QML plasmoids to access data stored by the library
 *
 * @author Francesco Nwokeka
 */

class DeclarativePlugins : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
};

Q_EXPORT_PLUGIN2(declarativeplugins, DeclarativePlugins)

#endif  // DECLARATIVEPLUGINS_H