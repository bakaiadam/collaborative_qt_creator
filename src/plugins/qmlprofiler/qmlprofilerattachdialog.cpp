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

#include "qmlprofilerattachdialog.h"
#include "ui_qmlprofilerattachdialog.h"

namespace QmlProfiler {
namespace Internal {

QmlProfilerAttachDialog::QmlProfilerAttachDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QmlProfilerAttachDialog)
{
    ui->setupUi(this);
}

QmlProfilerAttachDialog::~QmlProfilerAttachDialog()
{
    delete ui;
}

QString QmlProfilerAttachDialog::address() const
{
    return ui->addressLineEdit->text();
}

uint QmlProfilerAttachDialog::port() const
{
    return ui->portSpinBox->value();
}

QString QmlProfilerAttachDialog::sysroot() const
{
    return ui->sysrootChooser->path();
}

void QmlProfilerAttachDialog::setAddress(const QString &address)
{
    ui->addressLineEdit->setText(address);
}

void QmlProfilerAttachDialog::setPort(uint port)
{
    ui->portSpinBox->setValue(port);
}

void QmlProfilerAttachDialog::setSysroot(const QString &sysroot)
{
    ui->sysrootChooser->setPath(sysroot);
}

} // namespace Internal
} // namespace QmlProfiler
