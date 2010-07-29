/*
 * Copyright (C) 2010 Nokia Corporation <qt-info@nokia.com>
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "modelmenu.h"

#include <QtCore/QAbstractItemModel>
#include <KDebug>

ModelMenu::ModelMenu(QWidget * parent)
    : QMenu(parent)
    , m_maxRows(7)
    , m_firstSeparator(-1)
    , m_maxWidth(-1)
    , m_hoverRole(0)
    , m_separatorRole(0)
    , m_model(0)
{
    connect(this, SIGNAL(aboutToShow()), this, SLOT(aboutToShow()));
}

bool ModelMenu::prePopulated()
{
    kDebug();
    return false;
}

void ModelMenu::postPopulated()
{
    kDebug();
}

void ModelMenu::setModel(QAbstractItemModel *model)
{
    kDebug();
    m_model = model;
}

QAbstractItemModel *ModelMenu::model() const
{
    kDebug();
    return m_model;
}

void ModelMenu::setMaxRows(int max)
{
    kDebug();
    m_maxRows = max;
}

int ModelMenu::maxRows() const
{
    kDebug();
    return m_maxRows;
}

void ModelMenu::setFirstSeparator(int offset)
{
    kDebug();
    m_firstSeparator = offset;
}

int ModelMenu::firstSeparator() const
{
    kDebug();
    return m_firstSeparator;
}

void ModelMenu::setRootIndex(const QModelIndex &index)
{
    kDebug();
    m_root = index;
}

QModelIndex ModelMenu::rootIndex() const
{
    kDebug();
    return m_root;
}

void ModelMenu::setHoverRole(int role)
{
    kDebug();
    m_hoverRole = role;
}

int ModelMenu::hoverRole() const
{
    kDebug();
    return m_hoverRole;
}

void ModelMenu::setSeparatorRole(int role)
{
    kDebug();
    m_separatorRole = role;
}

int ModelMenu::separatorRole() const
{
    kDebug();
    return m_separatorRole;
}

Q_DECLARE_METATYPE(QModelIndex)
void ModelMenu::aboutToShow()
{
    kDebug();
    if (QMenu *menu = qobject_cast<QMenu*>(sender())) {
        QVariant v = menu->menuAction()->data();
        if (v.canConvert<QModelIndex>()) {
            QModelIndex idx = qvariant_cast<QModelIndex>(v);
            createMenu(idx, -1, menu, menu);
            disconnect(menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShow()));
            return;
        }
    }

    clear();
    if (prePopulated())
        addSeparator();
    int max = m_maxRows;
    if (max != -1)
        max += m_firstSeparator;
    createMenu(m_root, max, this, this);
    postPopulated();
}

void ModelMenu::createMenu(const QModelIndex &parent, int max, QMenu *parentMenu, QMenu *menu)
{
    kDebug();
    if (!menu) {
        QString title = parent.data().toString();
        menu = new QMenu(title, this);
        QIcon icon = qvariant_cast<QIcon>(parent.data(Qt::DecorationRole));
        menu->setIcon(icon);
        parentMenu->addMenu(menu);
        QVariant v;
        v.setValue(parent);
        menu->menuAction()->setData(v);
        connect(menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShow()));
        return;
    }

    int end = m_model->rowCount(parent);
    if (max != -1)
        end = qMin(max, end);

    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
    connect(menu, SIGNAL(hovered(QAction*)), this, SLOT(hovered(QAction*)));

    for (int i = 0; i < end; ++i) {
        QModelIndex idx = m_model->index(i, 0, parent);
        if (m_model->hasChildren(idx)) {
            createMenu(idx, -1, menu);
        } else {
            if (m_separatorRole != 0
                && idx.data(m_separatorRole).toBool())
                addSeparator();
            else
                menu->addAction(makeAction(idx));
        }
        if (menu == this && i == m_firstSeparator - 1)
            addSeparator();
    }
}

QAction *ModelMenu::makeAction(const QModelIndex &index)
{
    kDebug();
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QAction *action = makeAction(icon, index.data().toString(), this);
    QVariant v;
    v.setValue(index);
    action->setData(v);
    return action;
}

QAction *ModelMenu::makeAction(const QIcon &icon, const QString &text, QObject *parent)
{
    kDebug();
    QFontMetrics fm(font());
    if (-1 == m_maxWidth)
        m_maxWidth = fm.width(QLatin1Char('m')) * 30;
    QString smallText = fm.elidedText(text, Qt::ElideMiddle, m_maxWidth);
    return new QAction(icon, smallText, parent);
}

void ModelMenu::triggered(QAction *action)
{
    kDebug();
    QVariant v = action->data();
    if (v.canConvert<QModelIndex>()) {
        QModelIndex idx = qvariant_cast<QModelIndex>(v);
        emit activated(idx);
    }
}

void ModelMenu::hovered(QAction *action)
{
    kDebug();
    QVariant v = action->data();
    if (v.canConvert<QModelIndex>()) {
        QModelIndex idx = qvariant_cast<QModelIndex>(v);
        QString hoveredString = idx.data(m_hoverRole).toString();
        if (!hoveredString.isEmpty())
            emit hovered(hoveredString);
    }
}

