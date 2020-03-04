#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QtCore>

class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent = nullptr);
    const QString& ID() const {return _ID;};

signals:

protected:
    QString _ID;
};

class ControllerEvent{
public:
    explicit ControllerEvent(AbstractController* controller, const QString &key, const QVariant &value);
    AbstractController* controller() const {return _controller;};
    const QString& key() const {return _key;};
    const QVariant& value() const {return _value;};

private:
    AbstractController* _controller;
    const QString _key;
    QVariant _value;
};

#endif // ABSTRACTCONTROLLER_H
