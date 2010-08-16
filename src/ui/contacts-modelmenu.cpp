/*
* Copyright (C) 2010 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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


#include "contacts-modelmenu.h"
#include "contacts-list-model.h"
#include "grouped-contacts-proxy-model.h"

#include <KDebug>
#include "ontologies/imaccount.h"
#include "ontologies/personcontact.h"

using namespace KTelepathy;

ContactsListModel* ContactsModelMenu::s_ContactsModel = 0;
//GroupedContactsProxyModel* ContactsModelMenu::s_GroupedContactsModel = 0;

ContactsModelMenu::ContactsModelMenu(QString title, QWidget* parent)
    : ModelMenu(parent),
      m_data(*new QVariant()) // TODO memory leak?
{
    kDebug();
    connect(this, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(activated(const QModelIndex &)));
    setMaxRows(-1);

    if(!s_ContactsModel)
        s_ContactsModel = new ContactsListModel;

/*
    if(!s_GroupedContactsModel) {
        s_GroupedContactsModel = new GroupedContactsProxyModel;
        s_GroupedContactsModel->setSourceModel(s_ContactsModel);
    }
*/

    setModel(s_ContactsModel);
    setTitle(title);
}

void ContactsModelMenu::activated(const QModelIndex& index)
{
    kDebug() << index;
    kDebug() << index.data(ContactsListModel::PersonContactResourceRole);
    Nepomuk::PersonContact personcontact(index.data(ContactsListModel::PersonContactResourceRole).toUrl());
    kDebug() << personcontact.iMAccounts().first().imIDs().first();
    Q_EMIT activated(personcontact, m_data);
}

bool ContactsModelMenu::prePopulated()
{
    kDebug();
    return ModelMenu::prePopulated();
}

void ContactsModelMenu::postPopulated()
{
    kDebug();
    ModelMenu::postPopulated();
}

void ContactsModelMenu::createMenu(const QModelIndex& parent, int max, QMenu* parentMenu, QMenu* menu)
{
    kDebug() << parent << max << parentMenu << menu;
    ModelMenu::createMenu(parent, max, parentMenu, menu);
}

void ContactsModelMenu::setData(const QVariant& data)
{
    m_data = data;
}

QVariant& ContactsModelMenu::data() const
{
    return m_data;
}
