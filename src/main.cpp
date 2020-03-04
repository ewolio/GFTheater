#include <QtGui>
#include <QApplication>
#include <QSplashScreen>
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QLocale::setDefault(QLocale::English);

    // Show splash-screen
    QSplashScreen splash(QPixmap(":imgs/splash.png"));
    splash.show();
    app.processEvents();

    // Set dark style
    QFile f(":qdarkstyle/style.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = QTextStream(&f).readAll();
    f.close();

    QFile fVar(":qdarkstyle/styleConstant.txt");
    fVar.open(QFile::ReadOnly | QFile::Text);
    QTextStream varTS(&fVar);
    while(!varTS.atEnd()){
        QString line = varTS.readLine();
        if(line.startsWith("@")){
            QStringList var = line.split("=");
            styleSheet.replace(var[0].trimmed(), var[1].trimmed());
        }
    }
    fVar.close();


    app.setStyleSheet(styleSheet);

    // Build and show main window
    MainWindow::Instance()->show();
    splash.finish(MainWindow::Instance());


    return app.exec();
}
