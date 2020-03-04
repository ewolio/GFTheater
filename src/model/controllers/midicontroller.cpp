#include "midicontroller.h"

MidiController::MidiController()
{

}


// --------------------------------------------
MidiInterface *MidiInterface::_instance = NULL;

const MidiInterface& MidiInterface::Instance(){
    if(_instance == NULL){
        _instance = new MidiInterface();
        connect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
    }

    return *_instance;
}

void MidiInterface::resetInstance(){
    disconnect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
    delete _instance;
    _instance = NULL;
}

QStringList MidiInterface::listInDevice() const
{
    QStringList vec;
    int n = rtIn->getPortCount();
    for (int i=0; i<n; i++)
        vec.append(QString::fromStdString(rtIn->getPortName(i)));
    return vec;
}

MidiInterface::MidiInterface() : QObject()
{
    rtIn = new RtMidiIn();
    rtOut = new RtMidiOut();
}
