#include "showconfigdial.h"
#include "ui_showconfigdial.h"
#include "../../model/controllers/midicontroller.h"

ShowConfigDial::ShowConfigDial():
    QDialog(),
    ui(new Ui::ShowConfigDial),
    _updateMidiInterfaceTimer(this){
    ui->setupUi(this);
    auto panels = ui->panelSelector;
    panels->expandAll();
    int i = 0;
    auto item=panels->topLevelItem(0);
    while(item!=NULL){
        item->setData(0, Qt::UserRole, i++);
        item = panels->itemBelow(item);
    }
    panels->collapseAll();

    connect(ui->buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttons, SIGNAL(rejected()), this, SLOT(hide()));

    connect(ui->panelSelector, &QTreeWidget::itemSelectionChanged,
            [this] {ui->panels->setCurrentIndex(ui->panelSelector->currentItem()->data(0,Qt::UserRole).toInt());});

    setupMidi();
}

void ShowConfigDial::resizeEvent(QResizeEvent *) {

}

void ShowConfigDial::showEvent(QShowEvent *event) {
    event->accept();
    this->prepare();
    _updateMidiInterfaceTimer.start(4000);
}

void ShowConfigDial::hideEvent(QHideEvent *event)
{
    _updateMidiInterfaceTimer.stop();
}

/******************************************
 ------------  dial control  -------------
 *****************************************/
void ShowConfigDial::prepare(){
    resetFromConfig();
}

void ShowConfigDial::accept(){
    this->hide();
}

void ShowConfigDial::resetFromConfig(){
    resetMidi();
}

/******************************************
 ----------------  MIDI  ------------------
 *****************************************/
void ShowConfigDial::setupMidi() {
    connect(&_updateMidiInterfaceTimer, SIGNAL(timeout()),         this, SLOT(updateMidiInterfaces()));
    connect(ui->midiControllersView, SIGNAL(itemSelectionChanged()), this, SLOT(selectedMidiInterfaceChanged()));
}

void ShowConfigDial::resetMidi() {
    ui->midiControllersView->clearSelection();
    updateMidiInterfaces();
}

void ShowConfigDial::updateMidiInterfaces() {
    auto controllers = ui->midiControllersView;
    QString currentController = "";
    if(controllers->selectedItems().length())
        controllers->item(controllers->currentRow(), 1)->data(Qt::DisplayRole).toString();

    QStringList l = MidiInterface::Instance().listInDevice();
    controllers->setRowCount(l.length());
    for(int row=0; row<l.length(); row++) {
        QString device = l[row];
        auto deviceLabel = new QTableWidgetItem(device);
        deviceLabel->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        controllers->setItem(row, 1, deviceLabel);
        QCheckBox* deviceActiveCheckBox = new QCheckBox(controllers);
        controllers->setCellWidget(row, 0, deviceActiveCheckBox);

        if(currentController == device)
            controllers->selectRow(row);
    }

}

void ShowConfigDial::selectedMidiInterfaceChanged() {
    auto controllers = ui->midiControllersView;
    if(controllers->selectedItems().length()){
        QString name = controllers->item(controllers->currentRow(), 1)->data(Qt::DisplayRole).toString();
        ui->midiControllerProperties->setEnabled(true);
        ui->midiControllerName->setText(name);
    }else{
        ui->midiControllerName->setText(" - ");
        ui->midiControllerProperties->setEnabled(false);
    }
}

