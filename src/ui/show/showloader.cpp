#include "showloader.h"
#include "../mainwindow.h"
#include <quazip/JlCompress.h>

ShowLoader *ShowLoader::_instance = NULL;

ShowLoader* ShowLoader::Instance(){
    if(_instance==NULL){
        _instance = new ShowLoader();
        connect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
    }
    return _instance;
}
void ShowLoader::resetInstance(){
    disconnect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
    delete _instance;
    _instance = NULL;
}


ShowLoader::ShowLoader() :
    QObject()
{   
    qRegisterMetaType<ShowLoader::Action>("Projet::Action");

    _loaderThread = new QThread(this);

    _isSaved = true;
    _closeState = true;

    if(QDir(QDir::tempPath()+QDir::separator()+tmpFolderName).exists()){
        //-- Le logiciel a sans doute crashé, récupération du projet précédent --

        QMessageBox *mBox = new QMessageBox(QMessageBox::Warning,
                                           tr("Récupération de la session précédente"),
                                           tr("Le logiciel a du crasher lors de la dernière utilisation, voulez-vous tenter récuperer la session précédente?"),
                                           QMessageBox::Yes|QMessageBox::Ignore);
        int r = mBox->exec();
        if(r == QMessageBox::Yes)
            recover();
        else
            clearTmp();
    }

    moveToThread(_loaderThread);
    _loaderThread->start();
}

ShowLoader::~ShowLoader() {
    _loaderThread->terminate();
    _loaderThread->wait();
}

void ShowLoader::setupNew() {
    if(QThread::currentThread()!=_loaderThread){
        QMetaObject::invokeMethod(this, "setupNew", Qt::QueuedConnection);
        return;
    }
    changeAction(newProjet);

    if(!isClosed()){
        close();
        setupNew();
        return;
    }

    if(!createTmp())
        return fail();

    if(!readTmp())
        return fail();

    emit(opened());

    _closeState=false;
    _currentFile = NULL;

    success();
}


void ShowLoader::open(QFile* file){
    if(QThread::currentThread()!=_loaderThread){
        QMetaObject::invokeMethod(this, "open", Qt::QueuedConnection, Q_ARG(QFile*, file));
        return;
    }
    changeAction(openProjet);

    if(!isClosed()){
        close();
        open(file);
        return;
    }

    if(file == NULL)
        file = new QFile(QFileDialog::getOpenFileName());

    if(file==NULL)
        return fail();


    if(!fileToTmp(file))
        return fail();

    readTmp();

    emit(opened());
    _closeState = false;

    success();
}

void ShowLoader::save(QFile* file){

    if(QThread::currentThread()!=_loaderThread){
        QMetaObject::invokeMethod(this, "save", Qt::QueuedConnection, Q_ARG(QFile*, file));
        return;
    }
    changeAction(saveProjet);

    if(file == NULL){
        file = _currentFile!=NULL ? _currentFile : new QFile(QFileDialog::getSaveFileName(MainWindow::Instance(),
                                                                                        tr("Enregistrer le projet"),
                                                                                        QString(),
                                                                                        tr("Projet GFTheater (*.gfshow)")));
    }

    if(!compressToFile(file)){
        QMessageBox::critical(MainWindow::Instance(), tr("Erreur fatale lors de l'enregistrement"), tr("La sauvegarde n'a pu se terminer correctement"));
        return fail();
    }

    _currentFile = file;
    _isSaved = true;

    success();
}

void ShowLoader::saveAs(){
    QFile* file = new QFile(QFileDialog::getSaveFileName(MainWindow::Instance(), tr("Enregistrer le projet sous..."), QString(), tr("Projet GMoney (*.gmon)")));
    save(file);
}

void ShowLoader::recover()
{
    if(QThread::currentThread()!=_loaderThread){
        QMetaObject::invokeMethod(this, "recover", Qt::QueuedConnection);
        return;
    }
    changeAction(recoverProjet);
    if(!readTmp())
        return fail();
    success();
}

void ShowLoader::markAsNotSaved(){
    _isSaved = false;
}

void ShowLoader::close(){

    if(isClosed())
        return;

    if(QThread::currentThread()!=_loaderThread){
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
        return;
    }

    changeAction(closeProjet);

    if(!_isSaved){
        QMessageBox msgBox;
         msgBox.setText(tr("Les comptes ont été modifié"));
         msgBox.setInformativeText(tr("Voulez vous enregistrer les changements?"));
         msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         msgBox.setDefaultButton(QMessageBox::Save);
         switch(msgBox.exec()){
            case QMessageBox::Save:
                save();
                close();
                return;
            case QMessageBox::Cancel:
                return fail();
        }
    }



    clearTmp();

    emit closed();
    _closeState = true;

    success();
}



bool ShowLoader::isClosed(){
    return _closeState;
}

//________________________ GESTION DES FICHIERS TEMPORAIRES _____________________________

bool ShowLoader::createTmp()
{
    QDir(QDir::tempPath()).mkdir(tmpFolderName);
    ///TODO: initialiser les fichiers de projets
    return true;
}

bool ShowLoader::readTmp()
{
    ///TODO: Charger les fichiers du projets
    return true;
}

bool ShowLoader::clearTmp()
{
    return getTmpPath().removeRecursively();
}

bool ShowLoader::fileToTmp(QFile *file)
{
    return JlCompress::extractDir(file->fileName(), getTmpPath().path()).length()>0;
}

bool ShowLoader::compressToFile(QFile *file)
{
    return JlCompress::compressDir(file->fileName(), getTmpPath().path());
}

void ShowLoader::changeAction(ShowLoader::Action a)
{
    if(_action!=actionDone)
        fail();

    _action = a;
    emit(actionChanged(a));

}

void ShowLoader::fail()
{
    if(_action==actionDone)
        return;

    emit(actionFailed(_action));
    _action = actionDone;
}

void ShowLoader::success()
{if(_action==actionDone)
        return;

    emit(actionSucceed(_action));
    _action = actionDone;

}
