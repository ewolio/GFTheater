#include "consolewidget.h"
#include "../mainwindow.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    AbstractQMLComponent(QUrl("qrc:/console/console.qml"), parent)
{

}
