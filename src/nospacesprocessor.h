#ifndef NOSPACESPROCESSOR_H
#define NOSPACESPROCESSOR_H

#include "textprocessor.h"

class NoSpacesProcessor : public TextProcessor
{
    Q_OBJECT
public:
    NoSpacesProcessor(const QString & filename, QObject *parent = 0);
    virtual QString processorName() const;

    int detectLineCount();
public slots:
    virtual void processText();

};

#endif // NOSPACESPROCESSOR_H
