#ifndef CONFSHOW_H
#define CONFSHOW_H

#include <QtCore>

class ConfShow : public QObject
{
    Q_OBJECT
public:
    explicit ConfShow(QObject *parent = nullptr);

    bool load(const QJsonObject &json);
    bool save(QJsonObject &json) const;

signals:
    void changed();

};

#endif // CONFSHOW_H
