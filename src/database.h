#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

class Database : public QObject
{
    Q_OBJECT
public:

    Database( QObject * parent = 0 );
    virtual ~Database();
    bool isInitialized() const;

public slots:
    void initialize( const QString & filename );
    void open( const QString & filename );
    void addCombinationTable( const QString & name );
    void addFreqLenTable( const QString & name );
    void addCombination( const QString & table, const QString & combination, int lineNumber = -1, int startPos = -1, int endPos = -1 );

    void setInfo( const QString & key, const QString & data );

private:
    bool m_initialized;
};

#endif // DATABASE_H
