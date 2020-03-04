#ifndef VARIANTGRAPH_H
#define VARIANTGRAPH_H

#include <QtCore>

class AbstractNode;

class StateGraph : public QObject
{
    Q_OBJECT

public:
    explicit StateGraph(QObject *parent = nullptr);

    void registerVariant(AbstractNode* v);
    AbstractNode* operator[](const QString& name) const;
private:
    QMap<QString, AbstractNode*> _nodes;
};


class AbstractNode: public QObject
{
    Q_OBJECT

public:
    AbstractNode(QVector<AbstractNode*> dependencies, const QString& name);

    const QString& name() const {return _name;};

    const QVector<AbstractNode*>& children() const {return _children;};
    const QVector<AbstractNode*>& dependencies() const {return _dependencies;};
    QSet<AbstractNode*> allDependencies() const;
    int findDepId(AbstractNode* dep) const;
    int appendDep(AbstractNode* dep);
    void removeDep(AbstractNode* dep);
    void replaceDep(AbstractNode* oldDep, AbstractNode* newDep);
    void replaceDep(int depId, AbstractNode* newDep);

    const bool& isDirty() const {return _dirty;};
    void markAsDirty(AbstractNode* source=nullptr);

    const bool& isOverriden() const {return _override;};
    void clearOverride();
    virtual QVariant overrideToJson() const =0;
    virtual void overrideFromJson(const QVariant& json) =0;

protected:
    QVector<AbstractNode*> _children;
    bool _override = false;
    bool _dirty = false;
    virtual bool checkDirtyPropagation(int sourceId) const =0;

private:
    QString _name;
    QVector<AbstractNode*> _dependencies;
};

template <class T>
class Node: public AbstractNode
{
public:
    Node(T initialValue, QVector<AbstractNode*> dependencies, const QString& name);
    T value();
    void setOverride(const T &v);

protected:
    explicit Node(QVariant::Type type, QSet<Node*> dependencies, const QString& name);
    virtual T eval() const = 0;

private:
    T _eval_value;
    T _override_value;
};

template <class T>
Node<T>::Node(T initialValue, QVector<AbstractNode*> dependencies, const QString& name):
    AbstractNode(dependencies, name), _eval_value(initialValue){

}

template <class T>
T Node<T>::value() {
    if(_override)
        return _override_value;
    if(_dirty){
        _eval_value = eval();
        _dirty = false;
    }
    return _eval_value;
}

template <class T>
void Node<T>::setOverride(const T &v) {
    _override_value = v;
    _override = true;
}

#endif // VARIANTGRAPH_H
