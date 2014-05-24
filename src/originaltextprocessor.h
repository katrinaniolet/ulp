#ifndef ORIGINALTEXTPROCESSOR_H
#define ORIGINALTEXTPROCESSOR_H

#include "textprocessor.h"

class OriginalTextProcessor : public TextProcessor
{
    Q_OBJECT
public:
    enum Type {
        Unknown = 0,
        ProjectGutenberg
    };

    OriginalTextProcessor( const QString & filename, QObject * parent = 0 );
    Type type() const;

    QString processorName() const override;


public slots:
    void processText() override;

    Type detectType();
    QString detectTitle();
    QString detectAuthor();
    QString detectLanguage();
    int detectLineCount();

private:
    Type m_type;
};

#endif // ORIGINALTEXTPROCESSOR_H
