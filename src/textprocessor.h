#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QObject>

class TextProcessor : public QObject
{
    Q_OBJECT
public:
    explicit TextProcessor( const QString & filename, QObject *parent = 0);
    virtual QString processorName() const;

signals:
    void warning( const QString & message );
    void error( const QString & message );
    void progress( int value, int max = 100, const QString & message = "" );
    void infoFound( const QString & key, const QString & data );
    void lineFound( const QString & text, int lineNumber );
    void combinationFound( const QString & processor, const QString & text, int lineNumber = -1, int startPos = -1, int endPos = -1 );

public slots:
    virtual void processText();

protected:
    const QString m_filename;
};

#endif // TEXTPROCESSOR_H
