#include <QCoreApplication>
#include <iostream>
#include "processor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 2) {
        std::cout << qPrintable(QObject::tr("Usage: ulp [-r] <filename>")) << std::endl;
        std::cout << qPrintable(QObject::tr("\t-r\tRestart the process for the given filename")) << std::endl;
        std::cout << qPrintable(QObject::tr("\t\tinstead of resuming progress.")) << std::endl;
    }
    QString filename(QString::fromLocal8Bit(argv[1]));
    Processor processor(filename);

    return 0;//n a.exec();
}
