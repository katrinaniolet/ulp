#include <QFile>
#include <QStringList>
#include "originaltextprocessor.h"

OriginalTextProcessor::OriginalTextProcessor(const QString &filename, QObject *parent) :
    TextProcessor(filename, parent),
    m_type(Unknown)
{
}

QString OriginalTextProcessor::processorName() const
{
    return "OrignalWords";
}

OriginalTextProcessor::Type OriginalTextProcessor::detectType()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Could not open file %1").arg(m_filename));
        return Unknown;
    }
    while(!file.atEnd()) {
        if(file.readLine().startsWith("***START OF THE PROJECT GUTENBERG"))
            return ProjectGutenberg;
    }
    return Unknown;
}

QString OriginalTextProcessor::detectTitle()
{
    if(m_type == ProjectGutenberg) {
        QFile file(m_filename);
        if(!file.open(QIODevice::ReadOnly)) {
            emit error(tr("Could not open file %1").arg(m_filename));
            return tr("Unknown");
        }
        while(!file.atEnd()) {
            QByteArray line(file.readLine());
            if(line.startsWith("Title: ")) {
                QString out(QString::fromUtf8(line.replace("Title: ","").simplified()));
                emit infoFound("title",out);
                return out;
            }
        }
    }
    return tr("Unknown");
}

QString OriginalTextProcessor::detectAuthor()
{
    if(m_type == ProjectGutenberg) {
        QFile file(m_filename);
        if(!file.open(QIODevice::ReadOnly)) {
            emit error(tr("Could not open file %1").arg(m_filename));
            return tr("Unknown");
        }
        while(!file.atEnd()) {
            QByteArray line(file.readLine());
            if(line.startsWith("Author: ")) {
                QString out(QString::fromUtf8(line.replace("Author: ","").simplified()));
                emit infoFound("author",out);
                return out;
            }
        }
    }
    return tr("Unknown");
}

QString OriginalTextProcessor::detectLanguage()
{
    if(m_type == ProjectGutenberg) {
        QFile file(m_filename);
        if(!file.open(QIODevice::ReadOnly)) {
            emit error(tr("Could not open file %1").arg(m_filename));
            return tr("Unknown");
        }
        while(!file.atEnd()) {
            QByteArray line(file.readLine());
            if(line.startsWith("Language: ")) {
                QString out(QString::fromUtf8(line.replace("Language: ","").simplified()));
                emit infoFound("language",out);
                return out;
            }
        }
    }
    return tr("Unknown");
}

int OriginalTextProcessor::detectLineCount()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Could not open file %1").arg(m_filename));
        return 0;
    }

    int lineNumber(1);
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        if(m_type == ProjectGutenberg && line.startsWith("***START OF THE PROJECT GUTENBERG")) {
            lineNumber = 1;
        }
        else if(m_type == ProjectGutenberg && line.startsWith("***END OF THE PROJECT GUTENBERG")) {
            return lineNumber;
        }
        else {
             ++lineNumber;
        }
    }
    return lineNumber;
}

void OriginalTextProcessor::processText()
{
    if(m_type == Unknown) {
        m_type = detectType();
        if(m_type == Unknown)
            emit warning(tr("Could not detect file type, text info will not be available"));
    }

    emit progress(1,4,tr("Reading file info"));
    detectTitle();
    emit progress(2,4);
    detectAuthor();
    emit progress(3,4);
    detectLanguage();
    emit progress(4,4);

//    QFile outputFile(QFileInfo(m_textm_filename).baseName()+"-simplified.txt");
//    if(!outputFile.open(QIODevice::WriteOnly)) {
//        statusBar()->showMessage(tr("Could not open simplified output text file"));
//        return false;
//    }
//    QFile idealFile(QFileInfo(m_textm_filename).baseName()+"-idealoutput.txt");
//    if(!idealFile.open(QIODevice::WriteOnly)) {
//        statusBar()->showMessage(tr("Could not open ideal output text file"));
//        return false;
//    }

    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Could not open file %1").arg(m_filename));
        return;
    }

    if(m_type == ProjectGutenberg) {
        while(!file.atEnd()) {
            if(file.readLine().startsWith("***START OF THE PROJECT GUTENBERG"))
                break;
        }
    }

    const int totalLineCount = detectLineCount();
    int lineNumber(1);

    while(!file.atEnd()) {
        emit progress(lineNumber,totalLineCount,tr("Parsing original text"));
        QByteArray line = file.readLine();
        if(m_type == ProjectGutenberg && line.startsWith("***END OF THE PROJECT GUTENBERG"))
            break;

        emit lineFound(line, lineNumber);

        line.replace("."," ");
        line.replace("?"," ");
        line.replace("!"," ");
        line.replace("'"," ");
        line.replace("`"," ");
        line.replace("\""," ");
        line.replace(","," ");
        line.replace("-"," ");
        line.replace("+"," ");
        line.replace(";"," ");
        line.replace(":"," ");
        line.replace("("," ");
        line.replace(")"," ");
        line.replace("["," ");
        line.replace("]"," ");
        line.replace("{"," ");
        line.replace("}"," ");
        line.replace("\t"," ");
        line.replace("\n"," ");
        line.replace("\r"," ");
        line.replace("\v"," ");
        line.replace("\f"," ");
        //TODO replace non-visible characters

        line = line.simplified();
        if(line.isEmpty()) {
            ++lineNumber;
            continue;
        }

        QStringList words = QString::fromUtf8(line).split(" ");
        foreach(QString word, words) {
            word = word.simplified();
            if(!word.isEmpty()) {
                emit combinationFound(processorName(), word,lineNumber);
            }
        }
        ++lineNumber;
    }
    file.close();
}

