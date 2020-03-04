#ifndef MIDICONTROLLER_H
#define MIDICONTROLLER_H

#include <QtCore>
#include <RtMidi.h>
#include "controller.h"

class MidiController : public AbstractController
{
    Q_OBJECT
public:
    MidiController();


};


class MidiInterface : QObject {
    Q_OBJECT

public:
    static const MidiInterface& Instance();
    static void resetInstance();

    QStringList listInDevice() const;

private:
    MidiInterface();
    static MidiInterface* _instance;
    RtMidiIn* rtIn;
    RtMidiOut* rtOut;
};

#endif // MIDICONTROLLER_H
