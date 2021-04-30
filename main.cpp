#include "notepad.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Used for the -h and -v options
    a.setApplicationName("Notepad");
    a.setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("A simple text editor written in C++ with Qt");

    // Allow the opening of a file from the commandline with the -o or --open option
    QCommandLineOption file(QStringList() << "o" << "open", "Open the specified file", "file");
    parser.addOption(file);

    // Allow -h, -v, --help, --version and --help-all commandline options
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(a);

    Notepad w;

    // If a file is specified, open it
    QString fileName = parser.value(file);
    if(!fileName.isEmpty()) {
        w.open(fileName);
    }

    w.show();
    return a.exec();
}
