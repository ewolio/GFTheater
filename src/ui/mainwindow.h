#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlContext>
#include <QQmlEngine>
#include "show/showloader.h"
#include "show/showconfigdial.h"
#include "welcomepanel.h"
#include "console/consolewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* Instance();
    static void resetInstance();

    QQmlEngine* rootQmlEngine() const {return _qmlEngine;};

public slots:
    void showLoaderActionSucceed(ShowLoader::Action a);
    void showLoaderActionFailed(ShowLoader::Action a);

protected slots:
    void showOpened();
    void showClosed();
    void showSaved();
    void showNotSaved();

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *e);

private:
    MainWindow();
    static MainWindow* _instance;
    void setupUI();
    ~MainWindow();

    bool _quitResquested;

    Ui::MainWindow* _ui;
    ShowConfigDial* _showConfigDial;
    WelcomePanel* _welcomePanel;

    QQmlEngine* _qmlEngine;
    ConsoleWidget* _consoleWidget;

};

#endif // MAINWINDOW_H
