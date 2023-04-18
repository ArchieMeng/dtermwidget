/*  Copyright (C) 2008 e_k (e_k@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/


#ifndef _Q_TERM_WIDGET
#define _Q_TERM_WIDGET

#include <QPointer>
#include <QTranslator>
#include <QWidget>
#include <QPointer>
#include "Emulation.h"
#include "Filter.h"
#include "HistorySearch.h"
#include "Screen.h"

#include "qtermwidget_export.h"
#include "qtermwidget_version.h"
#include "qtermwidget_interface.h"

class QVBoxLayout;
class TermWidgetImpl;
class SearchBar;
class QUrl;
namespace Konsole {
class TerminalDisplay;
}

class QTERMWIDGET_EXPORT QTermWidget : public QWidget, public QTermWidgetInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.qtermwidget" FILE "qtermwidget.json")
    Q_INTERFACES(QTermWidgetInterface)

public:
    using KeyboardCursorShape = Konsole::Emulation::KeyboardCursorShape;

    //Creation of widget
    QTermWidget(int startnow, // 1 = start shell program immediately
                QWidget * parent = nullptr);
    // A dummy constructor for Qt Designer. startnow is 1 by default
    QTermWidget(QWidget *parent = nullptr);

    ~QTermWidget() override;

    //Initial size
    QSize sizeHint() const override;

    // expose TerminalDisplay::TerminalSizeHint, setTerminalSizeHint
    void setTerminalSizeHint(bool enabled) override;
    bool terminalSizeHint() override;

    //start shell program if it was not started in constructor
    void startShellProgram() override;

    // Returns session id list of processes running in the terminal window
        QList<int> getRunningSessionIdList();

    // Determine whether the terminal control has running process
    bool hasRunningProcess();

    /**
     * Start terminal teletype as is
     * and redirect data for external recipient.
     * It can be used for display and control a remote terminal.
     */
    void startTerminalTeletype() override;

    int getShellPID() override;

    // Get current terminal's session id
    int getSessionId();
    
    void changeDir(const QString & dir) override;

    // look-n-feel, if you don`t like defaults

    //  Terminal font
    // Default is application font with family Monospace, size 10
    // Beware of a performance penalty and display/alignment issues when using a proportional font.
    void setTerminalFont(const QFont & font) override;
    QFont getTerminalFont() override;
    void setTerminalOpacity(qreal level) override;
    void setTerminalBackgroundImage(const QString& backgroundImage) override;
    void setTerminalBackgroundMode(int mode) override;

    //environment
    void setEnvironment(const QStringList & environment) override;

    //  Shell program, default is /bin/bash
    void setShellProgram(const QString & program) override;

    //working directory
    void setWorkingDirectory(const QString & dir) override;
    QString workingDirectory() override;

    // Shell program args, default is none
    void setArgs(const QStringList & args) override;

    //Text codec, default is UTF-8
    void setTextCodec(QTextCodec * codec) override;

    void setTerminalWordCharacters(const QString &wc);

    /** @brief Sets the color scheme, default is white on black
     *
     * @param[in] name The name of the color scheme, either returned from
     * availableColorSchemes() or a full path to a color scheme.
     */
    void setColorScheme(const QString & name, bool needReloadTheme);
    void setColorScheme(const QString & name) override;

    /**
     * @brief Retrieves the available color schemes in the OS for the terminal.
     *
     * @note This function is needed in addition to the static one for making it availble when accessing QTermWidget as a plugin.
     *
     * @return A list of color schemes.
     */
    QStringList getAvailableColorSchemes() override;
    static QStringList availableColorSchemes();
    static void addCustomColorSchemeDir(const QString& custom_dir);

    /** Sets the history size (in lines)
     *
     * @param lines history size
     *  lines = 0, no history
     *  lies < 0, infinite history
     */
    void setHistorySize(int lines) override;

    // Returns the history size (in lines)
    int historySize() const override;

    // Presence of scrollbar
    void setScrollBarPosition(ScrollBarPosition) override;

    // Wrapped, scroll to end.
    void scrollToEnd() override;

    /******** Modify by n014361 wangpeili 2020-02-17:设置当前屏幕是否滚动****************/
    void setTrackOutput(bool enable);
    /********************* Modify by n014361 wangpeili End ************************/

    // Send some text to terminal
    void sendText(const QString & text) override;

    // Send key event to terminal
    void sendKeyEvent(QKeyEvent* e) override;

    // Sets whether flow control is enabled
    void setFlowControlEnabled(bool enabled) override;

    // Returns whether flow control is enabled
    bool flowControlEnabled(void) override;

    /**
     * Sets whether the flow control warning box should be shown
     * when the flow control stop key (Ctrl+S) is pressed.
     */
    void setFlowControlWarningEnabled(bool enabled) override;

    /*! Get all available keyboard bindings
     */
    static QStringList availableKeyBindings();

    //! Return current key bindings
    QString keyBindings() override;

    void setMotionAfterPasting(int) override;

    /** Return the number of lines in the history buffer. */
    int historyLinesCount() override;

    int screenColumnsCount() override;
    int screenLinesCount() override;

    /******** Modify by n014361 wangpeili 2020-02-13: 新增屏幕全选功能***********×****/
    void setSelectionAll();
    void setSelectionStart(int row, int column) override;
    void setSelectionEnd(int row, int column) override;
    void getSelectionStart(int& row, int& column) override;
    void getSelectionEnd(int& row, int& column) override;

    char getErase();
    /********************* Modify by ut000610 daizhengwen End ************************/

    // 设置删除键模式（修改内存接口）
    void setDeleteMode(char *key, int length);
    // 设置退格按键模式（用户选择接口）
    void setBackspaceMode(char *key, int length);

    /**
     * Returns the currently selected text.
     * @param preserveLineBreaks Specifies whether new line characters should
     * be inserted into the returned text at the end of each terminal line.
     */
    QString selectedText(bool preserveLineBreaks) override;
    QString selectedText(const Screen::DecodingOptions options=Screen::PreserveLineBreaks);

    void setMonitorActivity(bool) override;
    void setMonitorSilence(bool) override;
    void setSilenceTimeout(int seconds) override;

    /** Returns the available hotspot for the given point \em pos.
     *
     * This method may return a nullptr if no hotspot is available.
     *
     * @param[in] pos The point of interest in the QTermWidget coordinates.
     * @return Hotspot for the given position, or nullptr if no hotspot.
     */
    Filter::HotSpot* getHotSpotAt(const QPoint& pos) const;

    /** Returns the available hotspots for the given row and column.
     *
     * @return Hotspot for the given position, or nullptr if no hotspot.
     */
    Filter::HotSpot* getHotSpotAt(int row, int column) const;

    /*
     * Proxy for TerminalDisplay::filterActions
     * */
    QList<QAction*> filterActions(const QPoint& position) override;

    /**
     * Returns a pty slave file descriptor.
     * This can be used for display and control
     * a remote terminal.
     */
    int getPtySlaveFd() const override;

    /**
     * Sets the shape of the keyboard cursor.  This is the cursor drawn
     * at the position in the terminal where keyboard input will appear.
     */
    void setKeyboardCursorShape(KeyboardCursorShape shape);

    void setBlinkingCursor(bool blink) override;

    /** Enables or disables bidi text in the terminal. */
    void setBidiEnabled(bool enabled) override;
    bool isBidiEnabled() override;

    /**
     * Automatically close the terminal session after the shell process exits or
     * keep it running.
     */
    void setAutoClose(bool) override;

    QString title() const override;
    QString icon() const override;

    /** True if the title() or icon() was (ever) changed by the session. */
    bool isTitleChanged() const override;

    /** change and wrap text corresponding to paste mode **/
    void bracketText(QString& text) override;

    /** forcefully disable bracketed paste mode **/
    void disableBracketedPasteMode(bool disable) override;
    bool bracketedPasteModeIsDisabled() const override;

    /** Set the empty space outside the terminal */
    void setMargin(int) override;

    /** Get the empty space outside the terminal */
    int getMargin() const override;

    /** Get the foreground pid in terminal */
    int getForegroundProcessId() const;

    // 获取foreground名称
    QString getForegroundProcessName() const;

    void setDrawLineChars(bool drawLineChars) override;

    void setBoldIntense(bool boldIntense) override;

    void setConfirmMultilinePaste(bool confirmMultilinePaste) override;
    void setTrimPastedTrailingNewlines(bool trimPastedTrailingNewlines) override;
 
    // 获取是否允许输出时滚动
    bool getIsAllowScroll() const;

    // 设置是否允许输出时滚动
    void setIsAllowScroll(bool isAllowScroll);
    //Add by ut001000 renfeixiang 2020-12-02 当搜索框出现时，设置m_bHasSelect为false,
    //避免搜索框隐藏再显示之后，继续走m_bHasSelect为true流程，导致崩溃
    void setNoHasSelect();

    QTermWidgetInterface *createWidget(int startnow) const override;

signals:
    void finished();
    void copyAvailable(bool);

    void termGetFocus();
    void termLostFocus();
    void leftMouseClick();

    void termKeyPressed(QKeyEvent *);

    void urlActivated(const QUrl&, bool fromContextMenu);

    void bell(const QString& message);

    void activity();
    void silence();

    /******** Modify by nt001000 renfeixiang 2020-05-27:修改 增加参数区别remove和purge卸载命令 Begin***************/
    bool uninstallTerminal(QString commandname);
    /******** Modify by nt001000 renfeixiang 2020-05-27:修改 增加参数区别remove和purge卸载命令 Begin***************/

    /******** Modify by ut000610 daizhengwen 2020-06-11: QProcess start finished signal****************/
    void processStarted();
    /********************* Modify by ut000610 daizhengwen End ************************/

    /******** Modify by ut000610 daizhengwen 2020-09-03:将拖拽过来的数据传送给终端 Begin***************/
    void sendUrlsToTerm(const char *);
    /********************* Modify by ut000610 daizhengwen End ************************/
    /**
     * Emitted when emulator send data to the terminal process
     * (redirected for external recipient). It can be used for
     * control and display the remote terminal.
     */
    void sendData(const char *, int, const QTextCodec *);

    void profileChanged(const QString & profile);

    void titleChanged();

    // warning提示信息 currentShell当前使用的shell, 启用shell是否成功 true 替换了shell false 替换shell但启动失败
    void shellWarningMessage(QString currentShell, bool isSuccess);

    /**
     * Signals that we received new data from the process running in the
     * terminal emulator
     */
    void receivedData(const QString &text);

    /**
     * Signals for dynamically determine whether current terminal is busy or idle
     */
    void isTermIdle(bool bIdle);
    // 将库里返回信号透传出来。原来的no方法改名为clearSelection
    void sig_no();
    // 找到的信号
    void sig_();

    // 标签标题参数改变 dzw 2020-12-2
    void titleArgsChange(QString key, QString value);

public slots:
    // Copy selection to clipboard
    void copyClipboard();

    // Paste clipboard to terminal
    void pasteClipboard();

    // Paste selection to terminal
    void pasteSelection();

    // Set zoom
    void zoomIn();
    void zoomOut();

    // Set size
    void setSize(const QSize &);

    /*! Set named key binding for given widget
     */
    void setKeyBindings(const QString & kb);

    /*! Clear the terminal content and move to home position
     */
    void clear();

    void toggleShowSearchBar();
    // 清除选中框
    void clearSelection();

    void noMatchFound();
    /********************* Modify by n014361 wangpeili End ************************/

    void saveHistory(QIODevice *device);
protected:
    void resizeEvent(QResizeEvent *) override;

protected slots:
    void sessionFinished();
    void selectionChanged(bool textSelected);

private slots:
    void find();
    void findNext();
    void findPrevious();
    void matchFound(int startColumn, int startLine, int endColumn, int endLine);
    /**
     * Emulation::cursorChanged() signal propagates to here and QTermWidget
     * sends the specified cursor states to the terminal display
     */
    void cursorChanged(Konsole::Emulation::KeyboardCursorShape cursorShape, bool blinkingCursorEnabled);
    void snapshot();

private:
    void search(bool forwards, bool next);
    void setZoom(int step);
    void init(int startnow);
    void addSnapShotTimer();
    void interactionHandler();

    TermWidgetImpl *m_impl;
    SearchBar *m_searchBar;
    QVBoxLayout *m_layout;
    static QTranslator *m_translator;
    QPointer<Konsole::TerminalDisplay> m_termDisplay;
    QTimer *m_interactionTimer = nullptr;

    bool m_bHasSelect = false;
    int m_startColumn = 0;
    int m_startLine = 0;
    int m_endColumn = 0;
    int m_endLine = 0;
    int m_lastBackwardsPosition = -1;
    //上一次是正向搜索还是反向搜索
    bool m_isLastForwards = false;
};


//Maybe useful, maybe not

#ifdef __cplusplus
extern "C"
#endif
void * createTermWidget(int startnow, void * parent);

#endif

