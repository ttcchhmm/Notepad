#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    setupIcons();

    // Prevent the user from disabling the toolbar with a right click. Show the actions instead
    ui->toolBar->setContextMenuPolicy(Qt::ActionsContextMenu);

    // Use the system monospace font by default
    ui->textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

// Disable UI elements for unavaible features (clipboard and print support)
#ifndef QT_FEATURE_printer
    ui->actionPrint->setEnabled(false);
#endif

#ifndef QT_FEATURE_clipboard
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::on_actionNewDocument_triggered()
{
    if(checkClose()) {
        currentFile.clear();
        ui->textEdit->setText(QString());
        updateWindowTitle();
    }
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file");

    if(!fileName.isEmpty()) {
        open(fileName);
    }
}

void Notepad::on_actionSave_triggered()
{
    if(currentFile.isEmpty()) {
        currentFile = QFileDialog::getSaveFileName(this, "Save");

        updateWindowTitle();

        if(currentFile.isEmpty()) { // The user canceled the action
            return;
        }
    }

    save(currentFile);
}

void Notepad::on_actionSaveAs_triggered()
{
    currentFile = QFileDialog::getSaveFileName(this, "Save As");

    updateWindowTitle();

    if(currentFile.isEmpty()) { // The user canceled the action
        textChanged = true;
        return;
    }

    save(currentFile);
}

void Notepad::on_actionPrint_triggered()
{
#ifdef QT_FEATURE_printer
    QPrinter printDev;
#ifdef QT_FEATURE_printdialog
    QPrintDialog dialog(&printDev, this);
    if(dialog.exec() == QDialog::Rejected) {
        return;
    }
#endif // QT_FEATURE_printdialog
    ui->textEdit->print(&printDev);
#endif // QT_FEATURE_printer
}

void Notepad::on_actionCopy_triggered()
{
#ifdef QT_FEATURE_clipboard
    ui->textEdit->copy();
#endif
}

void Notepad::on_actionCut_triggered()
{
#ifdef QT_FEATURE_clipboard
    ui->textEdit->cut();
#endif
}

void Notepad::on_actionPaste_triggered()
{
#ifdef QT_FEATURE_clipboard
    ui->textEdit->paste();
#endif
}

void Notepad::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void Notepad::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void Notepad::on_actionFonts_triggered()
{
    bool selected;
    QFont font = QFontDialog::getFont(&selected, ui->textEdit->font(), this);

    if(selected) {
        ui->textEdit->setFont(font);
    }
}

void Notepad::on_textEdit_textChanged()
{
    textChanged = true;
}

void Notepad::open(QString fileName) {
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Notepad - Error", "Cannot open file: " + file.errorString());
        return;
    }

    currentFile = fileName;
    updateWindowTitle();
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();

    textChanged = false;
}

void Notepad::save(QString fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Notepad - Error", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    textChanged = false;
}

void Notepad::updateWindowTitle() {
    if(currentFile.isEmpty()) {
        setWindowTitle("Notepad");
    } else {
        setWindowTitle("Notepad - " + currentFile);
    }
}

// This work on Linux (tested on KDE Plasma), may break on other OSes
void Notepad::setupIcons() {
    ui->actionNewDocument->setIcon(QIcon::fromTheme("document-new"));
    ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
    ui->actionSave->setIcon(QIcon::fromTheme("document-save"));
    ui->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as"));
    ui->actionPrint->setIcon(QIcon::fromTheme("document-print"));
    ui->actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
    ui->actionCut->setIcon(QIcon::fromTheme("edit-cut"));
    ui->actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
    ui->actionUndo->setIcon(QIcon::fromTheme("edit-undo"));
    ui->actionRedo->setIcon(QIcon::fromTheme("edit-redo"));
    ui->actionFonts->setIcon(QIcon::fromTheme("font"));
    ui->actionAboutQt->setIcon(QIcon::fromTheme("help-about"));
}

// Prevent closing with unsaved work
void Notepad::closeEvent(QCloseEvent *qCE) {
    qCE->ignore();
    if(checkClose()) {
        qCE->accept();
    }
}

// If the document has been modified, ask the user for confirmation
bool Notepad::checkClose() {
    if(textChanged) {
        return QMessageBox::question(this, "Notepad - Confirmation", "You have unsaved work. Continue anyway ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
    } else {
        return true;
    }
}

void Notepad::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}
