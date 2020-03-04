#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *MainWindow::_instance = NULL;

MainWindow* MainWindow::Instance(){
    if(_instance == NULL){
        _instance = new MainWindow();
        connect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
        _instance->setupUI();
    }

    return _instance;
}
void MainWindow::resetInstance(){
    disconnect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
    delete _instance;
    _instance = NULL;
}


MainWindow::MainWindow() :
    QMainWindow(),
    _ui(new Ui::MainWindow)
{
    _welcomePanel = new WelcomePanel(this);
    _showConfigDial = new ShowConfigDial();
    _quitResquested = false;
    _qmlEngine = new QQmlEngine(this);

    _ui->setupUi(this);
    connect(_ui->showNew,    SIGNAL(triggered()), ShowLoader::Instance(), SLOT(setupNew()));
    connect(_ui->showOpen,   SIGNAL(triggered()), ShowLoader::Instance(), SLOT(open()));
    connect(_ui->showSave,   SIGNAL(triggered()), ShowLoader::Instance(), SLOT(save()));
    connect(_ui->showSaveAs, SIGNAL(triggered()), ShowLoader::Instance(), SLOT(saveAs()));
    connect(_ui->showClose,  SIGNAL(triggered()), ShowLoader::Instance(), SLOT(close()));

    connect(_ui->editShowOptions, SIGNAL(triggered()), _showConfigDial, SLOT(show()));

    connect(ShowLoader::Instance(), SIGNAL(opened()), this, SLOT(showOpened()));
    connect(ShowLoader::Instance(), SIGNAL(closed()), this, SLOT(showClosed()));
    connect(ShowLoader::Instance(), SIGNAL(saved()), this, SLOT(showSaved()));
    connect(ShowLoader::Instance(), SIGNAL(changesNotSaved()), this, SLOT(showNotSaved()));
    connect(ShowLoader::Instance(), SIGNAL(actionFailed(ShowLoader::Action)), this, SLOT(showLoaderActionFailed(ShowLoader::Action)));
    connect(ShowLoader::Instance(), SIGNAL(actionSucceed(ShowLoader::Action)), this, SLOT(showLoaderActionSucceed(ShowLoader::Action)));
    this->showClosed();

    connect(this, SIGNAL(destroyed()), qApp, SLOT(quit()));
    _welcomePanel->show();
}

void MainWindow::setupUI(){
    _consoleWidget = new ConsoleWidget(_ui->tabConsole);
    QHBoxLayout* consoleLayout = new QHBoxLayout(_ui->tabConsole);
    consoleLayout->addWidget(_consoleWidget);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::showOpened(){
    _ui->editShowOptions->setEnabled(true);
    _ui->showSaveAs->setEnabled(true);
    _ui->showClose->setEnabled(true);
}

void MainWindow::showClosed(){
    //ui->editShowOptions->setEnabled(false);
    _ui->showSave->setEnabled(false);
    _ui->showSaveAs->setEnabled(false);
    _ui->showClose->setEnabled(false);
}

void MainWindow::showSaved(){
    _ui->showSave->setEnabled(false);
}

void MainWindow::showNotSaved(){
    _ui->showSave->setEnabled(true);
}

void MainWindow::showLoaderActionSucceed(ShowLoader::Action a)
{
    switch(a){
    case ShowLoader::newProjet:
        break;
    case ShowLoader::openProjet:
        break;
    case ShowLoader::saveProjet:
        return;
    case ShowLoader::recoverProjet:
        break;
    case ShowLoader::closeProjet:
        if(_quitResquested)
            qApp->quit();
        return;
    }
    _quitResquested = false;
}

void MainWindow::showLoaderActionFailed(ShowLoader::Action a)
{
    switch(a){
    case ShowLoader::newProjet:
        break;
    case ShowLoader::openProjet:
        break;
    case ShowLoader::saveProjet:
        break;
    case ShowLoader::recoverProjet:
        break;
    case ShowLoader::closeProjet:
        return;
    }
    _quitResquested = false;
}


void MainWindow::resizeEvent(QResizeEvent *){
    _welcomePanel->setGeometry(width()/2-150, height()/2-100, 300, 200);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    ShowLoader::Instance()->close();
    e->accept();
}
