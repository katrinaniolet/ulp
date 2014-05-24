#include "textprocessor.h"

TextProcessor::TextProcessor(const QString &filename, QObject *parent) :
    QObject(parent),
    m_filename(filename)
{
}

QString TextProcessor::processorName() const
{
    return "unknown";
}

void TextProcessor::processText()
{
}
