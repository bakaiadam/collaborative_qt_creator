/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef ITEMLIBRARYTREEVIEW_H
#define ITEMLIBRARYTREEVIEW_H

#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>
#include <QDrag>
#include <QDebug>
#include <QTimeLine>
#include <QToolButton>
#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QFileSystemModel;
class QLabel;
QT_END_NAMESPACE

namespace QmlDesigner {

namespace Internal {

class ResourceItemDelegate;

// ItemLibraryTreeView with Drag implementation
class ItemLibraryTreeView : public QTreeView {
    Q_OBJECT
public:
    explicit ItemLibraryTreeView(QWidget *parent = 0);

    virtual void startDrag(Qt::DropActions supportedActions);
    virtual void setModel(QAbstractItemModel *model);

signals:
    void itemActivated(const QString &itemName);

private slots:
    void activateItem( const QModelIndex &index);

private:
    ResourceItemDelegate *m_delegate;
};

class ResourceItemDelegate : public QStyledItemDelegate
{
public:
    explicit ResourceItemDelegate(QObject *parent=0) :
        QStyledItemDelegate(parent),m_model(0) {}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    void setModel(QFileSystemModel *model);

private:
    QFileSystemModel *m_model;
};

} // namespace Internal

} // namespace QmlDesigner

#endif // ITEMLIBRARYTREEVIEW_H

