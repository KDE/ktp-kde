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

#include "account-item.h"

#include <KDebug>

using namespace KTelepathy;

AccountItem::AccountItem(Nepomuk::IMAccount imAccount, QObject* parent)
    : QObject(parent),
      AbstractTreeItem(),
      m_imAccount(imAccount),
      m_presenceIcon(new KIcon)
{
    kDebug();
    NepomukSignalWatcher* watcher = NepomukSignalWatcher::instance();
    watcher->registerCallbackOnSubject(m_imAccount, this);

    updatePresenceIcon();
}

AccountItem::~AccountItem()
{
    kDebug();
    NepomukSignalWatcher* watcher = NepomukSignalWatcher::instance();
    watcher->unregisterCallbackOnSubject(m_imAccount, this);
}

Nepomuk::IMAccount AccountItem::imAccount() const
{
    return m_imAccount;
}

QString AccountItem::displayName() const
{
    return m_imAccount.imNicknames().first();
}

QString AccountItem::accountIdentifier() const
{
    return m_imAccount.imIDs().first();
}

void AccountItem::updatePresenceIcon()
{
    // First, delete the old Icon.
    delete m_presenceIcon;

    // Now find out the current status.
    QList<qint64> statusTypes = m_imAccount.statusTypes();

    // If no presenceType set, then null KIcon.
    if (statusTypes.size() == 0) {
        m_presenceIcon = new KIcon();
        return;
    }

    // Get the presence type and set the icon appropriately from it.
    QString iconName;

    switch (statusTypes.first()) {
    case Tp::ConnectionPresenceTypeAvailable:
        iconName = QLatin1String("user-online");
        break;
    case Tp::ConnectionPresenceTypeAway:
        iconName = QLatin1String("user-away");
        break;
    case Tp::ConnectionPresenceTypeExtendedAway:
        iconName = QLatin1String("user-away-extended");
        break;
    case Tp::ConnectionPresenceTypeHidden:
        iconName = QLatin1String("user-invisible");
        break;
    case Tp::ConnectionPresenceTypeBusy:
        iconName = QLatin1String("user-busy");
        break;
    default:
        iconName = QLatin1String("user-offline");
        break;
    }

    m_presenceIcon = new KIcon(iconName);

/*  TODO Avatar

    kDebug() << "Attempt to build the avatar" << m_personContact.resourceUri();
    // Ok, now build the avatar
    m_pixmap = QPixmap();
    if (!m_personContact.avatarTokens().isEmpty()) {
        // Load the image then
        if (!m_personContact.photos().isEmpty()) {
            if (!m_personContact.photos().first().interpretedAses().isEmpty()) {
                QByteArray imgdata =
                QByteArray::fromBase64(
                                m_personContact.photos().first().interpretedAses().first().plainTextContents().first().toUtf8());
                QImage image = QImage::fromData(imgdata);
                m_pixmap = QPixmap::fromImage(image);
                m_pixmap = m_pixmap.scaled(32,32);
            }
        }
    }

    if (m_pixmap.isNull()) {
        // try to load the action icon
        m_pixmap = KIconLoader::global()->loadIcon("im-user",
                                                   KIconLoader::NoGroup,
                                                   32,
                                                   KIconLoader::DefaultState,
                                                   QStringList(),
                                                   0,
                                                   true);
    }

    // create a painter to paint the action icon over the key icon
    QPainter painter(&m_pixmap);
    // the the emblem icon to size 12
    int overlaySize = 12;
    // try to load the action icon
    const QPixmap iconPixmap = m_presenceIcon->pixmap(overlaySize);
    // if we're able to load the action icon paint it over
    if (!m_pixmap.isNull()) {
        QPoint startPoint;
        // bottom right corner
        startPoint = QPoint(32 - overlaySize - 1,
                            32 - overlaySize - 1);
        painter.drawPixmap(startPoint, iconPixmap);
    }
*/
}

const KIcon& AccountItem::presenceIcon() const
{
    Q_ASSERT(m_presenceIcon != 0);

    return *m_presenceIcon;
}

Tp::ConnectionPresenceType AccountItem::presenceType() const
{
    QList<qint64> statusTypes = m_imAccount.statusTypes();

    if (statusTypes.size() == 0)
        return Tp::ConnectionPresenceTypeUnset;

    return (Tp::ConnectionPresenceType)statusTypes.first();
}

void AccountItem::onStatementAdded(const Soprano::Statement &statement)
{
    Q_UNUSED(statement);

    updatePresenceIcon();
    Q_EMIT dirty();
}

/* TODO
const QPixmap& ContactItem::avatar() const
{
    return m_pixmap;
}
*/

#include "account-item.moc"








