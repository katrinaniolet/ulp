#ifndef IDEALOUTPUTFILEWRITER_H
#define IDEALOUTPUTFILEWRITER_H

#include "filewriter.h"

class IdealOutputFileWriter : public FileWriter
{
public:
    IdealOutputFileWriter(const QString & filenameBase, QObject * parent = 0);

public slots:
    void writeLine(QString line) override;
};

#endif // IDEALOUTPUTFILEWRITER_H
