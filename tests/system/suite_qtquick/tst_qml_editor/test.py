source("../../shared/qtcreator.py")

workingDir = None
templateDir = None

def main():
    global workingDir,templateDir
    sourceExample = os.path.abspath(sdkPath + "/Examples/4.7/declarative/keyinteraction/focus")
    if not neededFilePresent(sourceExample):
        return
    startApplication("qtcreator" + SettingsPath)
    # using a temporary directory won't mess up an eventually exisiting
    workingDir = tempDir()
    prepareTemplate(sourceExample)
    prepareForSignal("{type='CppTools::Internal::CppModelManager' unnamed='1'}", "sourceFilesRefreshed(QStringList)")
    createNewQtQuickApplication(workingDir, "untitled", templateDir + "/qml/focus.qml")
    # wait for parsing to complete
    waitForSignal("{type='CppTools::Internal::CppModelManager' unnamed='1'}", "sourceFilesRefreshed(QStringList)")
    testRenameId()
    testFindUsages()
    invokeMenuItem("File", "Exit")

def prepareTemplate(sourceExample):
    global templateDir
    templateDir = tempDir()
    templateDir = os.path.abspath(templateDir + "/template")
    shutil.copytree(sourceExample, templateDir)

def testRenameId():
    test.log("Testing rename of id")
    navTree = waitForObject("{type='Utils::NavigationTreeView' unnamed='1' visible='1' "
                            "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
    model = navTree.model()
    files = ["Core.ContextMenu\\.qml", "Core.GridMenu\\.qml", "Core.ListMenu\\.qml", "focus\\.qml"]
    originalTexts = {}
    editor = waitForObject("{type='QmlJSEditor::QmlJSTextEditorWidget' unnamed='1' visible='1' "
                           "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
    # temporarily store editor content for synchronizing purpose
    # usage of formerTxt is done because I couldn't get waitForSignal() to work
    # it always stored a different object into the signalObjects map as it looked up afterwards
    # although used objectMap.realName() for both
    formerTxt = editor.plainText
    for file in files:
        doubleClickFile(navTree, file)
        # wait until editor content switched to the double-clicked file
        while formerTxt==editor.plainText:
            editor = waitForObject("{type='QmlJSEditor::QmlJSTextEditorWidget' unnamed='1' visible='1' "
                                   "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
        # store content for next round
        formerTxt = editor.plainText
        originalTexts.setdefault(file, "%s" % formerTxt)
        test.log("stored %s's content" % file.replace("Core.","").replace("\\",""))
    # last opened file is the main file focus.qml
    line = "FocusScope\s*\{"
    if not placeCursorToLine(editor, line, True):
        test.fatal("File seems to have changed... Canceling current test")
        return False
    type(editor, "<Down>")
    openContextMenuOnTextCursorPosition(editor)
    ctxtMenu = waitForObject("{type='QMenu' visible='1' unnamed='1'}")
    activateItem(waitForObjectItem(objectMap.realName(ctxtMenu), "Rename Symbol Under Cursor"))
    type(waitForObject("{leftWidget={text='Replace with:' type='QLabel' unnamed='1' visible='1'} "
                       "type='Find::Internal::WideEnoughLineEdit' unnamed='1' visible='1' "
                       "window=':Qt Creator_Core::Internal::MainWindow'}"), "renamedView")
    clickButton(waitForObject("{text='Replace' type='QToolButton' unnamed='1' visible='1' "
                              "window=':Qt Creator_Core::Internal::MainWindow'}"))
    # store editor content for synchronizing purpose
    formerTxt = editor.plainText
    for file in files:
        doubleClickFile(navTree, file)
        # wait until editor content switched to double-clicked file
        while formerTxt==editor.plainText:
            editor = waitForObject("{type='QmlJSEditor::QmlJSTextEditorWidget' unnamed='1' visible='1' "
                                   "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
        # store content for next round
        formerTxt = editor.plainText
        originalText = originalTexts.get(file).replace("mainView", "renamedView")
        test.compare(originalText,formerTxt, "Comparing %s" % file.replace("Core.","").replace("\\",""))
    invokeMenuItem("File","Save All")

def __invokeFindUsage__(treeView, filename, line, typings, expectedCount):
    doubleClickFile(treeView, filename)
    editor = waitForObject("{type='QmlJSEditor::QmlJSTextEditorWidget' unnamed='1' visible='1' "
                       "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
    if not placeCursorToLine(editor, line, True):
        test.fatal("File seems to have changed... Canceling current test")
        return
    for ty in typings:
        type(editor, ty)
    openContextMenuOnTextCursorPosition(editor)
    ctxtMenu = waitForObject("{type='QMenu' visible='1' unnamed='1'}")
    activateItem(waitForObjectItem(objectMap.realName(ctxtMenu), "Find Usages"))
    validateSearchResult(expectedCount)

def testFindUsages():
    test.log("Testing find usage of an ID")
    navTree = waitForObject("{type='Utils::NavigationTreeView' unnamed='1' visible='1' "
                            "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
    __invokeFindUsage__(navTree, "focus\\.qml", "FocusScope\s*\{", ["<Down>"], 6)
    test.log("Testing find usage of a property")
    clickButton(waitForObject("{type='QToolButton' text='Clear' unnamed='1' visible='1' "
                              "window=':Qt Creator_Core::Internal::MainWindow'}"))
    __invokeFindUsage__(navTree, "focus\\.qml", "id: window", ["<Down>", "<Down>", "<Home>"], 26)

def validateSearchResult(expectedCount):
    searchResult = waitForObject(":Qt Creator_SearchResult_Core::Internal::OutputPaneToggleButton")
    ensureChecked(searchResult)
    resultTreeView = waitForObject("{type='Find::Internal::SearchResultTreeView' unnamed='1' "
                                   "visible='1' window=':Qt Creator_Core::Internal::MainWindow'}")
    counterLabel = waitForObject("{type='QLabel' unnamed='1' visible='1' text?='*matches found.' "
                                 "window=':Qt Creator_Core::Internal::MainWindow'}")
    matches = cast((str(counterLabel.text)).split(" ", 1)[0], "int")
    test.verify(matches==expectedCount, "Verfified match count.")
    model = resultTreeView.model()
    for row in range(model.rowCount()):
        index = model.index(row, 0)
        itemText = str(model.data(index).toString())
        doubleClickItem(resultTreeView, maskSpecialCharsForSearchResult(itemText), 5, 5, 0, Qt.LeftButton)
        test.log("%d occurrences in %s" % (model.rowCount(index), itemText))
        for chRow in range(model.rowCount(index)):
            chIndex = model.index(chRow, 0, index)
            resultTreeView.scrollTo(chIndex)
            text = str(chIndex.data())
            rect = resultTreeView.visualRect(chIndex)
            doubleClick(resultTreeView, rect.x+5, rect.y+5, 0, Qt.LeftButton)
            editor = waitForObject("{type='QmlJSEditor::QmlJSTextEditorWidget' unnamed='1' visible='1' "
                       "window=':Qt Creator_Core::Internal::MainWindow'}", 20000)
            line = lineUnderCursor(editor)
            test.compare(line, text)

def doubleClickFile(navTree, file):
    global templateDir
    treeElement = ("untitled.QML.%s/qml.%s" %
                   (maskSpecialCharsForProjectTree(templateDir),file))
    waitForObjectItem(navTree, treeElement)
    doubleClickItem(navTree, treeElement, 5, 5, 0, Qt.LeftButton)

def maskSpecialCharsForProjectTree(filename):
    filename = filename.replace("\\", "/").replace("_", "\\_").replace(".","\\.")
    # undoing mask operations on chars masked by mistake
    filename = filename.replace("/?","\\?").replace("/*","\\*")
    return filename

def maskSpecialCharsForSearchResult(filename):
    filename = filename.replace("_", "\\_").replace(".","\\.")
    return filename

def cleanup():
    global workingDir, templateDir
    waitForCleanShutdown()
    if workingDir!=None:
        deleteDirIfExists(workingDir)
    if templateDir!=None:
        deleteDirIfExists(os.path.dirname(templateDir))
