#ifndef CONTROLLERSHANDLER_H
#define CONTROLLERSHANDLER_H

#include <QtCore>
#include "controller.h"
#include "../stategraph.h"

class ControllersHandler: public QObject
{
    Q_OBJECT
public:
    static ControllersHandler* Instance();

public slots:
    void autoAssign(QString controlWidgetID);
    void stopAutoAssign(QString controlWidgetID);
    bool dispatchEvent(const ControllerEvent& event);

private:
    explicit ControllersHandler();
    static ControllersHandler* _instance;

    QSet<AbstractController*> _controllers;

};

class EventMap: public QObject {

    Q_OBJECT

public:
    EventMap(ControllersHandler* handler);

    QMap<QString, QMap<QString, StateGraph*>> map;
};

#endif // CONTROLLERSHANDLER_H
