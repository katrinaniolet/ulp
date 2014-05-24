#include "nospacesfilewriter.h"

NoSpacesFileWriter::NoSpacesFileWriter(const QString &filenameBase, QObject *parent) :
    FileWriter(filenameBase+"-nospaces.txt", parent)
{
}

void NoSpacesFileWriter::writeLine(QString line)
{
    line.replace(".","");
    line.replace("?","");
    line.replace("!","");
    line.replace("'","");
    line.replace("`","");
    line.replace("\"","");
    line.replace(",","");
    line.replace("-","");
    line.replace("+","");
    line.replace(";","");
    line.replace(":","");
    line.replace("(","");
    line.replace(")","");
    line.replace("[","");
    line.replace("]","");
    line.replace("{","");
    line.replace("}","");
    line.replace("\t","");
    line.replace("\n","");
    line.replace("\r","");
    line.replace("\v","");
    line.replace("\f","");
    line.replace(" ","");

    FileWriter::writeLine(line);
}
