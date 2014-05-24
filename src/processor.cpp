#include <QFile>
#include <QFileInfo>
#include <iostream>
#include "database.h"
#include "processor.h"
#include "originaltextprocessor.h"
#include "idealoutputfilewriter.h"
#include "nospacesfilewriter.h"
#include "nospacesprocessor.h"

Processor::Processor(const QString &filename, QObject *parent) :
    QObject(parent),
    m_filename(filename),
    m_lastPercentage(0),
    m_database(new Database(this))
{
    verifyInputFile();
    createDatabase();
    loadText();
}

void Processor::createDatabase()
{
    m_database->initialize(QString("%1.sqlite").arg(QFileInfo(m_filename).baseName()));
}

void Processor::loadText()
{
    Q_ASSERT(m_database);
    OriginalTextProcessor originalProcessor(m_filename);
    IdealOutputFileWriter ideal(QFileInfo(m_filename).baseName());
    NoSpacesFileWriter noSpaces(QFileInfo(m_filename).baseName());

    m_database->addCombinationTable(originalProcessor.processorName());
    //m_database->addFreqLenTable("OriginalWordsFreqLen");

    connect(&originalProcessor, SIGNAL(progress(int,int,QString)), this, SLOT(showProgress(int,int,QString)));
    connect(&originalProcessor, SIGNAL(infoFound(QString,QString)), m_database, SLOT(setInfo(QString,QString)));
    connect(&originalProcessor, SIGNAL(lineFound(QString,int)), this, SLOT(addLineFromOriginal(QString,int)));
    connect(&originalProcessor, SIGNAL(combinationFound(QString, QString,int,int,int)), m_database, SLOT(addCombination(QString,QString,int,int,int)));

    connect(&originalProcessor, SIGNAL(lineFound(QString,int)), &ideal, SLOT(writeLine(QString)));
    connect(&originalProcessor, SIGNAL(lineFound(QString,int)), &noSpaces, SLOT(writeLine(QString)));

    originalProcessor.processText();

    NoSpacesProcessor nsp(noSpaces.filename());
    m_database->addCombinationTable(nsp.processorName());

    connect(&nsp, SIGNAL(progress(int,int,QString)), this, SLOT(showProgress(int,int,QString)));
    connect(&nsp, SIGNAL(combinationFound(QString, QString,int,int,int)), m_database, SLOT(addCombination(QString,QString,int,int,int)));

    nsp.processText();
}

void Processor::showProgress(int current, int max, const QString &message)
{
    int percentage = static_cast<int>(qRound((static_cast<float>(current)/static_cast<float>(max))*100));
    if(percentage <= m_lastPercentage && (message.isEmpty() || message==m_lastProgressMessage))
        return;
    m_lastPercentage = percentage;

    if(!message.isEmpty())
        m_lastProgressMessage = message;

    std::cout << qPrintable(m_lastProgressMessage) << ": " << m_lastPercentage << "%" << std::endl;
}

void Processor::verifyInputFile()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly))
        qFatal("Could not open input file");
    file.close();
}
