#ifndef SHOW_H
#define SHOW_H

#include <QtCore>
#include "config/confshow.h"

class Show : public QObject
{
    Q_OBJECT
public:
    explicit Show(QObject *parent = nullptr);

    void save_show(const QDir &path);
    void load_show(const QDir &path);

    const ConfShow& configuration() const {return *_config;};

signals:
    void changed();


protected:
    ConfShow* _config;
    bool readJSON(const QDir &path, QString filename, QJsonObject& obj);
    bool writeJSON(const QDir &path, QString filename, const QJsonObject& obj);
};

#endif // SHOW_H
