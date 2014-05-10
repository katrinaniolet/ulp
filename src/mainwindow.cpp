#include <QFile>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoad_Text,SIGNAL(triggered()),this,SLOT(loadText()));
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
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        if(line.startsWith("Title: "))
            ui->textTitle->setText(QString::fromUtf8(line.replace("Title: ","").simplified()));
        else if(line.startsWith("Language: "))
            ui->textLanguage->setText(QString::fromUtf8(line.replace("Language: ","").simplified()));
        else if(line.startsWith("***START OF THE PROJECT GUTENBERG"))
            break;
    }

    if(!createDatabase())
        return false;


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

    //Create database file
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_databaseFilename);
    if(!db.open()) {
        statusBar()->showMessage(tr("Could not create database"));
        return false;
    }

    //Create tables and indexes
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
