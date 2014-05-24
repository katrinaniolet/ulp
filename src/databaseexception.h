#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <QException>

class DatabaseException : public QException
{
public:
    DatabaseException( const QString & message ) throw();
    ~DatabaseException() throw() {}
    void raise() const override { throw *this; }
    DatabaseException *clone() const override { return new DatabaseException(*this); }
    QString message() const throw();
    const char* what() const throw() override;
private:
    const QString m_message;
};

#endif // DATABASEEXCEPTION_H
