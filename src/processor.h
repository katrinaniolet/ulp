#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
class Database;

class Processor : public QObject
{
    Q_OBJECT
public:
    explicit Processor(const QString & filename, QObject *parent = 0);

signals:

public slots:
    void verifyInputFile();

    void createDatabase();
    void loadText();

    void showProgress(int current, int max, const QString & message);

private:
    const QString m_filename;
    QString m_lastProgressMessage;
    int m_lastPercentage;
    Database * m_database;
};

#endif // PROCESSOR_H
