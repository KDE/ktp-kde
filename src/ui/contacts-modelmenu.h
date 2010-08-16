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

#ifndef LIBKTELEPATHY_CONTACTS_MODELMENU_H
#define LIBKTELEPATHY_CONTACTS_MODELMENU_H

#include <kdemacros.h>

#include <modelmenu.h>
#include <contacts-list-model.h>

namespace Nepomuk {
    class PersonContact;
}

namespace KTelepathy {
class ContactsListModel;
class GroupedContactsProxyModel;

//TODO allow to use a proxy model instead of the default model
class KDE_EXPORT ContactsModelMenu: public ModelMenu
{
    Q_OBJECT

public:
    ContactsModelMenu(QString title = "", QWidget* parent = 0);

    void setData(const QVariant& data);
    QVariant& data() const;

protected:
    bool prePopulated();
    void postPopulated();
    void createMenu(const QModelIndex& parent, int max, QMenu* parentMenu = 0, QMenu* menu = 0);

private Q_SLOTS:
    void activated(const QModelIndex& index);

Q_SIGNALS:
    void activated(Nepomuk::PersonContact& personContact, QVariant& data);

private:
    static ContactsListModel* s_ContactsModel;
//    static GroupedContactsProxyModel* s_GroupedContactsModel;
    QVariant& m_data;
};

} // namespace KTelepathy

#endif // LIBKTELEPATHY_CONTACTS_MODELMENU_H
