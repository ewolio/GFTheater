#include "show.h"

Show::Show(QObject *parent) :
    QObject(parent)
{
    _config = new ConfShow(this);
    connect(_config, SIGNAL(changed()), this, SIGNAL(changed()));
}

void Show::load_show(const QDir &path) {
    QJsonObject json;

    if(readJSON(path, "config", json))
        _config->load(json);
}

void Show::save_show(const QDir &path) {
    QJsonObject json;

    if (_config->save(json))
        writeJSON(path, "config", json);
}

bool Show::readJSON(const QDir &path, QString filename, QJsonObject& obj)
{
    if(!filename.endsWith(".json"))
        filename += ".json";

    if(!path.exists(filename))
        return false;

    QFile file(path.filePath(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Impossible to load %1.").arg(filename);
        return false;
    }

    QJsonParseError err;
    const QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &err));
    if(err.error){
        qWarning() << QString("Error when parsing %1:\n%2").arg(filename).arg(err.errorString());
        return false;
    }

    file.close();
    obj = doc.object();
    return true;
}

bool Show::writeJSON(const QDir &path, QString filename, const QJsonObject &obj)
{
    if(!filename.endsWith(".json"))
        filename += ".json";

    QFile file(path.filePath(filename));
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << QString("Impossible to write in %1.").arg(filename);
        return false;
    }

    QJsonDocument doc(obj);
    file.write(doc.toJson());
    file.close();
    return true;
}
