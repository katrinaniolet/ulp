#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    bool loadText();
    bool createDatabase();

private:
    Ui::MainWindow *ui;
    QString m_textFilename;
    QString m_databaseFilename;
};

#endif // MAINWINDOW_H
