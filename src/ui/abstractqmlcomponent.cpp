#include "abstractqmlcomponent.h"
#include "mainwindow.h"

AbstractQMLComponent::AbstractQMLComponent(const QUrl&  qmlDef, QWidget* parent):
    QQuickWidget(MainWindow::Instance()->rootQmlEngine(), parent)
{
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    setSource(qmlDef);
}
