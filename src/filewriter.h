#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>
#include <QFile>

class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(const QString & filenameBase, QObject *parent = 0);

    QString filename() const;

signals:

public slots:
    virtual void writeLine(QString line );

private:
    QString m_filename;
    QFile m_file;

};

#endif // FILEWRITER_H
