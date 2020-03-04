#include "controllershandler.h"

ControllersHandler *ControllersHandler::_instance = NULL;

ControllersHandler* ControllersHandler::Instance(){
    if(_instance == NULL){
        _instance = new ControllersHandler();
        connect(qApp, SIGNAL(aboutToQuit()), _instance, SLOT(deleteLater()));
    }
    return _instance;
}

void ControllersHandler::autoAssign(QString controlWidgetID)
{

}

void ControllersHandler::stopAutoAssign(QString controlWidgetID)
{

}

bool ControllersHandler::dispatchEvent(const ControllerEvent &event)
{

}

ControllersHandler::ControllersHandler() : QObject() {

}
