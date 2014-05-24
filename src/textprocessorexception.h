#ifndef TEXTPROCESSOREXCEPTION_H
#define TEXTPROCESSOREXCEPTION_H

#include <QException>

class TextProcessorException : public QException
{
public:
    TextProcessorException( const QString & message ) throw();
    ~TextProcessorException() throw() {}
    void raise() const override { throw *this; }
    TextProcessorException *clone() const override { return new TextProcessorException(*this); }
    QString message() const throw();
    const char* what() const throw() override;
private:
    const QString m_message;
};

#endif // TEXTPROCESSOREXCEPTION_H
