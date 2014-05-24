#include <QString>
#include "databaseexception.h"

DatabaseException::DatabaseException(const QString &message) throw() :
    m_message(message)
{
}

QString DatabaseException::message() const throw()
{
    return message();
}

const char *DatabaseException::what() const throw()
{
    return qPrintable(message());
}
