#include "filewriter.h"

FileWriter::FileWriter(const QString &filenameBase, QObject *parent) :
    QObject(parent),
    m_filename(filenameBase)
{
}

QString FileWriter::filename() const
{
    return m_filename;
}

void FileWriter::writeLine( QString line )
{
    if(!m_file.isOpen()) {
        m_file.setFileName(m_filename);
        if(!m_file.open(QIODevice::WriteOnly))
            qFatal(qPrintable(tr("Could not open file %1 for writing").arg(m_filename)));
    }
    line.replace("\n","");
    line.replace("\r","");
    if(!line.isEmpty()) {
        m_file.write(line.toUtf8());
        m_file.write("\n");
    }
}
