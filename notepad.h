#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QFontDatabase>
#include <QCloseEvent>

// Disable support for unavaible features if needed
#if defined (QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>

#ifdef QT_FEATURE_printer
#include <QPrinter>

#ifdef QT_FEATURE_printdialog
#include <QPrintDialog>
#endif // QT_FEATURE_printdialog

#endif // QT_FEATURE_printer
#endif // defined (QT_PRINTSUPPORT_LIB)

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

    void open(QString fileName);

protected:
    void closeEvent(QCloseEvent *qCE) override;

private slots:
    void on_actionNewDocument_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionPrint_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionFonts_triggered();

    void on_textEdit_textChanged();

    void on_actionAboutQt_triggered();

private:
    void save(QString fileName);
    void updateWindowTitle();
    void setupIcons();
    bool checkClose();

    Ui::Notepad *ui;
    QString currentFile;
    bool textChanged = false;
};
#endif // NOTEPAD_H
