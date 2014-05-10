#include <QFile>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoad_Text,SIGNAL(triggered()),this,SLOT(loadText()));
    connect(ui->actionLoad_Database,SIGNAL(triggered()),this,SLOT(loadDatabase()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadText()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select text"), QString(), tr("Text files (*.txt)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        statusBar()->showMessage(tr("Could not open file %1").arg(fileName));
        return false;
    }
    m_textFilename = fileName;

    if(!createDatabase())
        return false;

    QString title;
    QString author;
    QString language;

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        if(line.startsWith("Title: ")) {
            title = QString::fromUtf8(line.replace("Title: ","").simplified());
            ui->textTitle->setText(title);
        }
        else if(line.startsWith("Author: ")) {
            author = QString::fromUtf8(line.replace("Author: ","").simplified());
        }
        else if(line.startsWith("Language: ")) {
            language = QString::fromUtf8(line.replace("Language: ","").simplified());
            ui->textLanguage->setText(language);
        }
        else if(line.startsWith("***START OF THE PROJECT GUTENBERG"))
            break;
    }

    QSqlQuery infoQuery;
    infoQuery.prepare("INSERT INTO INFO (TITLE, AUTHOR, LANGUAGE, FILENAME) VALUES (:title, :author, :language, :filename)");
    infoQuery.bindValue(":title",title);
    infoQuery.bindValue(":author",author);
    infoQuery.bindValue(":language",language);
    infoQuery.bindValue(":filename",QFileInfo(m_textFilename).baseName());
    if(!infoQuery.exec()) {
        statusBar()->showMessage(tr("Database error: %1").arg(infoQuery.lastError().databaseText()));
        return false;
    }

    return true;
}

bool MainWindow::createDatabase()
{
    if(m_textFilename.isEmpty()) {
        statusBar()->showMessage(tr("Text not loaded"));
        return false;
    }
    QFileInfo file(m_textFilename);
    if(!file.exists()) {
        statusBar()->showMessage(tr("Text file missing"));
        return false;
    }
    m_databaseFilename = QString("%1.sqlite").arg(file.baseName());
    QFile dbFile(m_databaseFilename);
    if(dbFile.exists())
        dbFile.remove();

    //Create database file
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_databaseFilename);
    if(!db.open()) {
        statusBar()->showMessage(tr("Could not create database"));
        return false;
    }

    //Create tables and indexes
    db.exec("CREATE TABLE INFO ("
            "TITLE          TEXT,"
            "AUTHOR         TEXT,"
            "LANGUAGE       TEXT,"
            "FILENAME       TEXT"
            ");");
    db.exec("CREATE TABLE ORIGINAL ("
            "COMBINATION    TEXT    NOT NULL,"
            "LINENUMBER     INT     NOT NULL,"
            "STARTPOS       INT     NOT NULL,"
            "ENDPOS         INT     NOT NULL"
            ");");
    db.exec("CREATE TABLE COMBINATIONS ("
            "COMBINATION    TEXT    NOT NULL,"
            "LINENUMBER     INT     NOT NULL,"
            "STARTPOS       INT     NOT NULL,"
            "ENDPOS         INT     NOT NULL"
            ");");
    db.exec("CREATE INDEX idx_ORIGINAL ON ORIGINAL (COMBINATION ASC);");
    db.exec("CREATE INDEX idx_COMBINATIONS ON COMBINATIONS (COMBINATION ASC);");
    return true;
}

bool MainWindow::loadDatabase()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select database"), QString(), tr("SQLite files (*.sqlite)"));
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if(!db.open()) {
        statusBar()->showMessage(tr("Could not open database"));
        return false;
    }

    QSqlQuery infoQuery("SELECT TITLE, LANGUAGE FROM INFO LIMIT 1;");
    if(!infoQuery.exec()) {
        statusBar()->showMessage(tr("Could not load text info from database"));
        return false;
    }

    while(infoQuery.next()) {
        ui->textTitle->setText(infoQuery.value("TITLE").toString());
        ui->textLanguage->setText(infoQuery.value("LANGUAGE").toString());
    }
    m_databaseFilename = fileName;
    return true;
}
