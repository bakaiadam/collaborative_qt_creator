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

#include "qtuicodemodelsupport.h"
#include "qt4buildconfiguration.h"

#include "qt4project.h"
#include "qt4target.h"
#include <qtsupport/baseqtversion.h>

using namespace Qt4ProjectManager;
using namespace Internal;

Qt4UiCodeModelSupport::Qt4UiCodeModelSupport(CPlusPlus::CppModelManagerInterface *modelmanager,
                                             Qt4Project *project,
                                             const QString &source,
                                             const QString &uiHeaderFile)
    : CppTools::UiCodeModelSupport(modelmanager, source, uiHeaderFile),
      m_project(project)
{

}

Qt4UiCodeModelSupport::~Qt4UiCodeModelSupport()
{

}

QString Qt4UiCodeModelSupport::uicCommand() const
{
    Qt4BuildConfiguration *qt4bc = m_project->activeTarget()->activeQt4BuildConfiguration();
    if (!qt4bc->qtVersion())
        return QString();
    return qt4bc->qtVersion()->uicCommand();
}

QStringList Qt4UiCodeModelSupport::environment() const
{
    Qt4BuildConfiguration *qt4bc = m_project->activeTarget()->activeQt4BuildConfiguration();
    return qt4bc->environment().toStringList();
}
