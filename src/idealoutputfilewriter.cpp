#include "idealoutputfilewriter.h"

IdealOutputFileWriter::IdealOutputFileWriter(const QString &filenameBase, QObject *parent) :
    FileWriter(filenameBase+"-ideal.txt", parent)
{
}

void IdealOutputFileWriter::writeLine( QString line )
{
    line.replace("."," ");
    line.replace("?"," ");
    line.replace("!"," ");
    line.replace("'"," ");
    line.replace("`"," ");
    line.replace("\""," ");
    line.replace(","," ");
    line.replace("-"," ");
    line.replace("+"," ");
    line.replace(";"," ");
    line.replace(":"," ");
    line.replace("("," ");
    line.replace(")"," ");
    line.replace("["," ");
    line.replace("]"," ");
    line.replace("{"," ");
    line.replace("}"," ");
    line.replace("\t"," ");
    line.replace("\n"," ");
    line.replace("\r"," ");
    line.replace("\v"," ");
    line.replace("\f"," ");
    line = line.simplified();

    FileWriter::writeLine(line);
}
