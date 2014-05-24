#include <QFile>
#include "nospacesprocessor.h"

NoSpacesProcessor::NoSpacesProcessor(const QString &filename, QObject *parent) :
    TextProcessor(filename,parent)
{
}

QString NoSpacesProcessor::processorName() const
{
    return "NoSpaces";
}

int NoSpacesProcessor::detectLineCount()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Could not open file %1").arg(m_filename));
        return 0;
    }

    int lineNumber(1);
    while(!file.atEnd()) {
        file.readLine();
        ++lineNumber;
    }
    return lineNumber;
}


void NoSpacesProcessor::processText()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Could not open file %1").arg(m_filename));
        return;
    }

    const int totalLineCount = detectLineCount();
    int lineNumber(1);

    while(!file.atEnd()) {
        emit progress(lineNumber,totalLineCount,tr("Parsing text with no spaces"));
        QByteArray line = file.readLine();

        emit lineFound(line, lineNumber);

        line = line.simplified();
        if(line.isEmpty()) {
            ++lineNumber;
            continue;
        }

        for(int pos=0;pos<line.count();++pos) {
            emit progress(pos,line.count(),tr("Line %1 of %2").arg(lineNumber).arg(totalLineCount));
            for(int len=1;len<14;++len) {
                emit combinationFound(processorName(),line.mid(pos,len),lineNumber);
            }
        }
        //emit combinationFound(processorName(), word,lineNumber);
        ++lineNumber;
    }
    file.close();

}
