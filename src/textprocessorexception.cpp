#include <QString>
#include "textprocessorexception.h"

TextProcessorException::TextProcessorException(const QString & message) throw() :
    m_message(message)
{
}

QString TextProcessorException::message() const throw()
{
    return m_message;
}

const char *TextProcessorException::what() const throw()
{
    return qPrintable(m_message);
}
