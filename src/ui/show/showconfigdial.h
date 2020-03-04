#ifndef CONFIG_PROJET_H
#define CONFIG_PROJET_H

#include <QtGui>
#include <QtWidgets>
#include "showloader.h"

namespace Ui {
class ShowConfigDial;
}


class ShowConfigDial: public QDialog
{
    Q_OBJECT

public:
    ShowConfigDial();

public slots:
    void prepare();
    void accept();

protected slots:
    void resetFromConfig();

protected:
    ShowLoader* showLoader;
    Ui::ShowConfigDial* ui;
    QDialogButtonBox* buttons;

    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent  *event);
    void hideEvent(QHideEvent  *event);

    // MIDI
    void setupMidi();
    void resetMidi();
    QTimer _updateMidiInterfaceTimer;
protected slots:
    void selectedMidiInterfaceChanged();
    void updateMidiInterfaces();
};


#endif // CONFIG_PROJET_H
