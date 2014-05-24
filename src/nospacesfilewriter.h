#ifndef NOSPACESFILEWRITER_H
#define NOSPACESFILEWRITER_H

#include "filewriter.h"

class NoSpacesFileWriter : public FileWriter
{
public:
    NoSpacesFileWriter(const QString & filenameBase, QObject * parent = 0);

public slots:
    void writeLine( QString line ) override;
};

#endif // NOSPACESFILEWRITER_H
