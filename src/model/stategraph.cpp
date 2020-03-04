#include "stategraph.h"

StateGraph::StateGraph(QObject *parent) : QObject(parent)
{

}

void StateGraph::registerVariant(AbstractNode *v) {
    if(_nodes.contains(v->name())){
        qWarning() << QString("The variant graph already contains a node name %1.").arg(v->name());
        return;
    }
    _nodes[v->name()] = v;
}


AbstractNode *StateGraph::operator[](const QString &name) const {
    return _nodes.value(name, 0);
}


/*************************************************
 ----------------  VariantNode  ------------------
 *************************************************/

AbstractNode::AbstractNode(QVector<AbstractNode*> dependencies, const QString& name):
    QObject(), _name(name), _dependencies()
{
    foreach(auto dependence, dependencies)
        appendDep(dependence);
}

QSet<AbstractNode*> AbstractNode::allDependencies() const {
    QSet<AbstractNode*> deps;
    foreach(auto dep, _dependencies){
        deps.insert(dep);
        deps.unite(dep->allDependencies());
    }
    return deps;
}

int AbstractNode::findDepId(AbstractNode *dep) const {
    return _dependencies.indexOf(dep);
}

int AbstractNode::appendDep(AbstractNode *dep) {
    assert(dep->allDependencies().contains(this));
    dep->_children.append(this);
    _dependencies.append(dep);
    markAsDirty(dep);
    return _dependencies.length() - 1;
}

void AbstractNode::removeDep(AbstractNode *dep) {
    int depId = findDepId(dep);
    if(depId > -1)
        _dependencies.remove(depId);
}

void AbstractNode::replaceDep(AbstractNode *oldDep, AbstractNode *newDep) {
    int depId = findDepId(oldDep);
    if(depId > -1){
        oldDep->_children.removeOne(this);
        newDep->_children.append(this);
        _dependencies.replace(depId, newDep);
        markAsDirty(newDep);
    }
}

void AbstractNode::replaceDep(int depId, AbstractNode *newDep) {
    AbstractNode* oldDep = _dependencies[depId];
    oldDep->_children.removeOne(this);
    newDep->_children.append(this);
    _dependencies.replace(depId, newDep);
    markAsDirty(newDep);
}

void AbstractNode::markAsDirty(AbstractNode* source) {
    if(_dirty)
        return;
    if(source!=nullptr){
        if(!checkDirtyPropagation(findDepId(source)))
            return;
    }
    _dirty = true;

    if(!_override) {
        foreach(auto child, _children)
            child->markAsDirty(this);
    }
}

void AbstractNode::clearOverride()
{
    _override = false;
    if(_dirty) {
        foreach(auto child, _children)
            child->markAsDirty(this);
    }
}
