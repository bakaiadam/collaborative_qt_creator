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

#ifndef BASETEXTDOCUMENT_H
#define BASETEXTDOCUMENT_H

#include "texteditor_global.h"

#include <coreplugin/textfile.h>


#include "basetextdocumentlayout.h"
#include "basetexteditor.h"
#include "typingsettings.h"
#include "storagesettings.h"
#include "tabsettings.h"
#include "extraencodingsettings.h"
#include "syntaxhighlighter.h"
#include "texteditorconstants.h"

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QFutureInterface>
#include <QtGui/QMainWindow>
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QApplication>

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <utils/qtcassert.h>
#include <utils/fileutils.h>
#include <utils/reloadpromptutils.h>


QT_BEGIN_NAMESPACE
class QTextCursor;
class QTextDocument;
QT_END_NAMESPACE

namespace TextEditor {

namespace Internal{
class DocumentMarker : public ITextMarkable
{
    Q_OBJECT
public:
    DocumentMarker(QTextDocument *);

    TextMarks marks() const { return m_marksCache; }

    // ITextMarkable
    bool addMark(ITextMark *mark, int line);
    TextMarks marksAt(int line) const;
    void removeMark(ITextMark *mark);
    bool hasMark(ITextMark *mark) const;
    void updateMark(ITextMark *mark);

private:
    double recalculateMaxMarkWidthFactor() const;

    TextMarks m_marksCache; // not owned
    QTextDocument *document;
};

}
class BaseTextDocumentPrivate
{
public:
    explicit BaseTextDocumentPrivate(BaseTextDocument *q);

    QString m_fileName;
    QString m_defaultPath;
    QString m_suggestedFileName;
    QString m_mimeType;
    TypingSettings m_typingSettings;
    StorageSettings m_storageSettings;
    TabSettings m_tabSettings;
    ExtraEncodingSettings m_extraEncodingSettings;
    QTextDocument *m_document;
    Internal::DocumentMarker *m_documentMarker;
    SyntaxHighlighter *m_highlighter;

    bool m_fileIsReadOnly;
    bool m_hasHighlightWarning;

    int m_autoSaveRevision;
};

class ITextMarkable;
class TypingSettings;
class StorageSettings;
class TabSettings;
class ExtraEncodingSettings;
class SyntaxHighlighter;
class BaseTextDocumentPrivate;

class TEXTEDITOR_EXPORT BaseTextDocument : public Core::TextFile
{
    Q_OBJECT

public:
    BaseTextDocumentPrivate *d;
    BaseTextDocument();
    virtual ~BaseTextDocument();

    void setTypingSettings(const TypingSettings &typingSettings);
    void setStorageSettings(const StorageSettings &storageSettings);
    void setTabSettings(const TabSettings &tabSettings);
    void setExtraEncodingSettings(const ExtraEncodingSettings &extraEncodingSettings);

    const TypingSettings &typingSettings() const;
    const StorageSettings &storageSettings() const;
    const TabSettings &tabSettings() const;
    const ExtraEncodingSettings &extraEncodingSettings() const;

    ITextMarkable *documentMarker() const;

    // IFile implementation.
    virtual bool save(QString *errorString, const QString &fileName, bool autoSave);
    virtual QString fileName() const;
    virtual bool shouldAutoSave() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;
    virtual bool isSaveAsAllowed() const;
    virtual void checkPermissions();
    bool reload(QString *errorString, ReloadFlag flag, ChangeType type);
    virtual QString mimeType() const;
    void setMimeType(const QString &mt);
    virtual void rename(const QString &newName);

    virtual QString defaultPath() const;
    virtual QString suggestedFileName() const;

    void setDefaultPath(const QString &defaultPath);
    void setSuggestedFileName(const QString &suggestedFileName);

    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    virtual bool reload(QString *errorString);

    QTextDocument *document() const;
    void setSyntaxHighlighter(SyntaxHighlighter *highlighter);
    SyntaxHighlighter *syntaxHighlighter() const;

    bool reload(QString *errorString, QTextCodec *codec);
    void cleanWhitespace(const QTextCursor &cursor);

    bool hasHighlightWarning() const;
    void setHighlightWarning(bool has);

signals:
    void titleChanged(QString title);

private:
    void cleanWhitespace(QTextCursor &cursor, bool cleanIndentation, bool inEntireDocument);
    void ensureFinalNewLine(QTextCursor &cursor);
    void documentClosing();

};

} // namespace TextEditor

#endif // BASETEXTDOCUMENT_H
