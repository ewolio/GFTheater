#include "welcomepanel.h"
#include "ui_welcomepanel.h"
#include "show/showloader.h"

WelcomePanel::WelcomePanel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::welcomePanel)
{
    ui->setupUi(this);

    connect(ui->newB,  SIGNAL(clicked()), ShowLoader::Instance(), SLOT(setupNew()));
    connect(ui->openB, SIGNAL(clicked()), ShowLoader::Instance(), SLOT(open()));

    connect(ShowLoader::Instance(), SIGNAL(opened()), this, SLOT(hide()));
    connect(ShowLoader::Instance(), SIGNAL(closed()), this, SLOT(show()));
}

WelcomePanel::~WelcomePanel()
{
    delete ui;
}

void WelcomePanel::showEvent(QShowEvent *){
    while(ui->recentLayout->count()>0){
        QLayoutItem* l = ui->recentLayout->itemAt(0);
        ui->recentLayout->removeItem(l);
        delete l->widget();
        delete l;
    }

    QSettings s;
    foreach (QString i, s.value("App/recentFiles").toStringList()){
        QPushButton* b = new QPushButton(QFileInfo(i).baseName(),this);
        b->setFlat(true);
        b->setStyleSheet("text-align: left;");
        ui->recentLayout->addWidget(b);
        connect(b,SIGNAL(clicked()),this, SLOT(recentButtonsClicked()));
    }

    ui->recentLayout->addStretch();

    ui->statusLabel->setHidden(true);
    ui->newB->setEnabled(true);
    ui->openB->setEnabled(true);
    ui->groupBox->setEnabled(true);
}

void WelcomePanel::setupForLoading(QFile *f){
    ui->newB->setEnabled(false);
    ui->openB->setEnabled(false);
    ui->groupBox->setEnabled(false);

    ui->statusLabel->setVisible(true);
    if(f!=NULL && f!=0)
        ui->statusLabel->setText("Loading show: <i>"+QFileInfo(*f).fileName());
    else
        ui->statusLabel->setText("Loading show");

    this->repaint();
}

void WelcomePanel::recentButtonsClicked(){

    int i=ui->recentLayout->indexOf(qobject_cast<QWidget *>(sender()));

    QSettings s;

    if(i>=s.value("App/recentFiles").toStringList().size())
        return;

    ShowLoader::Instance()->open(new QFile(s.value("App/recentFiles").toStringList().at(i)));


}
