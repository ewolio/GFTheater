#ifndef ABSTRACTQMLCOMPONENT_H
#define ABSTRACTQMLCOMPONENT_H

#include <QQuickWidget>

class AbstractQMLComponent : public QQuickWidget
{
    Q_OBJECT
public:
    AbstractQMLComponent(const QUrl &qmlDef, QWidget *parent=NULL);
};

#endif // ABSTRACTQMLCOMPONENT_H
