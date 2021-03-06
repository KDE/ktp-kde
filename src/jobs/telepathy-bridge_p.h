/*
 * This file is part of telepathy-contactslist-prototype
 *
 * Copyright (C) 2009-2010 Collabora Ltd. <info@collabora.co.uk>
 *   @Author Dario Freddi <dario.freddi@collabora.co.uk>
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

#ifndef LIBKTELEPATHY_TELEPATHY_BRIDGE_P_H
#define LIBKTELEPATHY_TELEPATHY_BRIDGE_P_H

#include "telepathy-bridge.h"

#include <TelepathyQt4/AccountManager>

namespace KTelepathy {

class TelepathyAccountProxy;

class TelepathyBridgePrivate
{
    Q_DECLARE_PUBLIC(TelepathyBridge)
    TelepathyBridge * const q_ptr;
public:
    explicit TelepathyBridgePrivate(TelepathyBridge *parent) : q_ptr(parent) {}
    ~TelepathyBridgePrivate() {}

    Tp::AccountManagerPtr accountManager;
    QList< TelepathyAccountProxy* > accountProxies;
    QUrl mePimoURI;

    TelepathyAccountProxy *accountProxyForAccount(const Nepomuk::IMAccount &resource) const;
    QList< TelepathyAccountProxy* > accountProxiesForContact(const Nepomuk::PersonContact &contact) const;
    QList< Nepomuk::PersonContact > contactsForMetaContact(const Nepomuk::Person &metacontact) const;

    void __k__onAccountManagerReady(Tp::PendingOperation* op);
    void __k__onAccountCreated(const QString &path);
};

} // namespace KTelepathy

#endif // LIBKTELEPATHY_TELEPATHY_BRIDGE_P_H
