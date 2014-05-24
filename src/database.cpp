#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QVariant>
#include <QObject>
#include <QDebug>
#include "database.h"
#include "databaseexception.h"

Database::Database(QObject *parent) :
    QObject(parent),
    m_initialized(false)
{
}

Database::~Database()
{
}

bool Database::isInitialized() const
{
    return m_initialized;
}

void Database::initialize(const QString &filename)
{
    if(isInitialized()) {
        throw DatabaseException(QObject::tr("Database already initialized"));
    }

    if(filename.isEmpty()) {
        throw DatabaseException(QObject::tr("Invalid database filename: %1").arg(filename));
    }

    //Remove the file if it exists
    QFile dbFile(filename);
    if(dbFile.exists())
        dbFile.remove();

    //Create database file
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    if(!db.open()) {
        throw DatabaseException(QObject::tr("Could not create database: %1").arg(filename));
    }

    //Create info table
    db.exec("CREATE TABLE INFO ("
            "KEY            TEXT,"
            "DATA           TEXT"
            ");");

    m_initialized = true;
}

void Database::open(const QString &filename)
{
    if(isInitialized())
        throw DatabaseException(QObject::tr("Database already initialized"));

    if(filename.isEmpty())
        throw DatabaseException(QObject::tr("Invalid database filename: %1").arg(filename));

    QFile dbFile(filename);
    if(!dbFile.exists())
        throw DatabaseException(QObject::tr("Invalid database filename: %1").arg(filename));
}

void Database::addCombinationTable(const QString &name)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        throw DatabaseException(QObject::tr("Could not open database: %1").arg(db.lastError().databaseText()));
    }
    QString query(QString("CREATE TABLE %1 ("
                          "COMBINATION    TEXT    NOT NULL,"
                          "LINENUMBER     INT     NOT NULL,"
                          "STARTPOS       INT,"
                          "ENDPOS         INT"
                          ");").arg(name));
    db.exec(query);
    QString indexQuery((QString("CREATE INDEX idx_%1 ON %1 (COMBINATION ASC);")).arg(name));
    db.exec(indexQuery);
    db.close();
    addFreqLenTable(name+"FreqLen");
}

void Database::addFreqLenTable(const QString &name)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        throw DatabaseException(QObject::tr("Could not open database: %1").arg(db.lastError().databaseText()));
    }
    QString query(QString("CREATE TABLE %1 ("
                          "COMBINATION    TEXT    NOT NULL,"
                          "LENGTH         INT     NOT NULL,"
                          "FREQUENCY      INT     NOT NULL"
                          ");").arg(name));
    db.exec(query);
    QString indexQuery((QString("CREATE UNIQUE INDEX idx_%1 ON %1 (COMBINATION ASC);")).arg(name));
    db.exec(indexQuery);
}



void Database::addCombination(const QString & table, const QString &combination, int lineNumber, int startPos, int endPos)
{

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        throw DatabaseException(QObject::tr("Could not open database: %1").arg(db.lastError().databaseText()));
    }

    QSqlQuery infoQuery;
    QString query(QString("INSERT INTO %1 (COMBINATION, LINENUMBER, STARTPOS, ENDPOS) VALUES (:combo, :linenumber, :startpos, :endpos)").arg(table));
    infoQuery.prepare(query);
    infoQuery.bindValue(":combo",combination.simplified());
    infoQuery.bindValue(":linenumber",lineNumber);
    infoQuery.bindValue(":startpos",startPos);
    infoQuery.bindValue(":endpos",endPos);
    if(!infoQuery.exec()) {
        throw DatabaseException(QObject::tr("Database error: %1").arg(infoQuery.lastError().databaseText()));
    }

    QSqlQuery freqQuery;
    QString query2(QString("INSERT OR REPLACE INTO %1 VALUES (:combo, :len, "
                          "COALESCE((SELECT FREQUENCY FROM %2 "
                          "WHERE COMBINATION=:combo2),"
                          "0) + 1);").arg(table+"FreqLen").arg(table+"FreqLen"));
   freqQuery.prepare(query2);
   freqQuery.bindValue(":combo", combination);
   freqQuery.bindValue(":len", combination.count());
   freqQuery.bindValue(":combo2", combination);
   if(!freqQuery.exec()) {
       qDebug() << infoQuery.lastError().databaseText() << infoQuery.lastError().driverText();
       throw DatabaseException(QObject::tr("Database error: %1").arg(infoQuery.lastError().databaseText()));
   }

}

void Database::setInfo(const QString &key, const QString &data)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open()) {
        throw DatabaseException(QObject::tr("Could not open database: %1").arg(db.lastError().databaseText()));
    }
    QSqlQuery infoQuery;
    infoQuery.prepare("INSERT OR REPLACE INTO INFO (KEY, DATA) VALUES (:key, :data)");
    infoQuery.bindValue(":key",key);
    infoQuery.bindValue(":data",data);
    if(!infoQuery.exec()) {
        throw DatabaseException(QObject::tr("Database error: %1").arg(infoQuery.lastError().databaseText()));
    }
}
