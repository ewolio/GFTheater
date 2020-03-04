#ifndef PROJET_H
#define PROJET_H

#include <QtCore>
#include "../../model/show.h"


class ShowLoader : public QObject
{
    Q_OBJECT
public:
    static ShowLoader* Instance();
    static void resetInstance();

    QThread *getProjectThread(){return _loaderThread;}

    bool isClosed();

    enum Action{
        newProjet,
        openProjet,
        saveProjet,
        recoverProjet,
        closeProjet,
        actionDone
    };

    Action currentAction(){return _action;}
    QDir getTmpPath(){return QDir(QDir::tempPath()+QDir::separator()+tmpFolderName);};

signals:
    void closed();
    void opened();
    void changesNotSaved();
    void saved();

    void actionChanged(ShowLoader::Action a);
    void actionSucceed(ShowLoader::Action a);
    void actionFailed(ShowLoader::Action a);

public slots:
    void setupNew();
    void open(QFile* file=NULL);
    void saveAs();
    void save(QFile* file=NULL);
    void recover();
    void close();

    void markAsNotSaved();

protected:
    const QString tmpFolderName = "GFTheater";

    ~ShowLoader();

private:
    ShowLoader();

    bool readTmp();
    bool createTmp();
    bool fileToTmp(QFile *file);
    bool compressToFile(QFile *file);
    bool clearTmp();

    void changeAction(Action a);
    void fail();
    void success();

    QThread *_loaderThread;
    Action _action;

    QFile* _currentFile;
    Show* _currentShow;

    bool _isSaved;
    bool _closeState;

    static ShowLoader* _instance;
};


#endif // PROJET_H
