#include <QFile>
#include <QFileDialog>
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

void MainWindow::loadText()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select text"), QString(), tr("Text files (*.txt)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        statusBar()->showMessage(tr("Could not open file %1").arg(fileName));
        return;
    }
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        if(line.startsWith("Title: "))
            ui->textTitle->setText(QString::fromUtf8(line.replace("Title: ","").simplified()));
        else if(line.startsWith("Language: "))
            ui->textLanguage->setText(QString::fromUtf8(line.replace("Language: ","").simplified()));
        else if(line.startsWith("***START OF THE PROJECT GUTENBERG"))
            break;
    }
}
