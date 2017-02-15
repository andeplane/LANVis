#include "selectionmodifier.h"
#include <QDebug>

SelectionModifier::SelectionModifier(Modifier *parent) : Modifier(parent),
    m_min(0), m_max(0)
{

}

template <int x, int y, int z, int type, int property, int operatorType>
void SelectionModifier::doApply(const State &state, std::vector<int> &particleIndices, std::vector<bool> &selected)
{
    if(selected.size() != particleIndices.size()) {
        qDebug() << "Error, selected array not same size as particles.";
        return;
    }

    for(int i=0; i<particleIndices.size(); i++) {
        const IdentifiableParticle &particle = state.allParticles()[particleIndices[i]];
        if(x==1) {
            if(operatorType==GREATER) selected[i] = selected[i] || particle.position.x() > m_min;
//            else if(operatorType==LESS) selected[i] |= particles[i].position.x() < m_max;
//            else if(operatorType==EQUAL) selected[i] |= particles[i].position.x() == m_min;
//            else if(operatorType==GREATER) selected[i] |= particles[i].position.x() > m_min && particles[i].position.x() < m_max;
        } else if(y==1) {
//            if(operatorType==GREATER) selected[i] |= particles[i].position.y() > m_min;
//            else if(operatorType==LESS) selected[i] |= particles[i].position.y() < m_max;
//            else if(operatorType==EQUAL) selected[i] |= particles[i].position.y() == m_min;
//            else if(operatorType==GREATER) selected[i] |= particles[i].position.y() > m_min && particles[i].position.y() < m_max;
        }
    }
}

void SelectionModifier::apply(const State &state, std::vector<int> &particleIndices, std::vector<bool> &selected)
{
    if(m_propertyName.toLower()=="x") {
        if(m_operatorName=="Greater") doApply<1,0,0,0,0,GREATER>(state,particleIndices,selected);
        if(m_operatorName=="Less")    doApply<1,0,0,0,0,LESS>(state,particleIndices,selected);
        if(m_operatorName=="Equal")   doApply<1,0,0,0,0,EQUAL>(state,particleIndices,selected);
        if(m_operatorName=="Range")   doApply<1,0,0,0,0,RANGE>(state,particleIndices,selected);
    } else if(m_propertyName.toLower()=="y") {
        if(m_operatorName=="Greater") doApply<0,1,0,0,0,GREATER>(state,particleIndices,selected);
        if(m_operatorName=="Less")    doApply<0,1,0,0,0,LESS>(state,particleIndices,selected);
        if(m_operatorName=="Equal")   doApply<0,1,0,0,0,EQUAL>(state,particleIndices,selected);
        if(m_operatorName=="Range")   doApply<0,1,0,0,0,RANGE>(state,particleIndices,selected);
    } else if(m_propertyName.toLower()=="z") {
        if(m_operatorName=="Greater") doApply<0,0,1,0,0,GREATER>(state,particleIndices,selected);
        if(m_operatorName=="Less")    doApply<0,0,1,0,0,LESS>(state,particleIndices,selected);
        if(m_operatorName=="Equal")   doApply<0,0,1,0,0,EQUAL>(state,particleIndices,selected);
        if(m_operatorName=="Range")   doApply<0,0,1,0,0,RANGE>(state,particleIndices,selected);
    } else if(m_propertyName.toLower()=="type") {
        if(m_operatorName=="Greater") doApply<0,0,0,1,0,GREATER>(state,particleIndices,selected);
        if(m_operatorName=="Less")    doApply<0,0,0,1,0,LESS>(state,particleIndices,selected);
        if(m_operatorName=="Equal")   doApply<0,0,0,1,0,EQUAL>(state,particleIndices,selected);
        if(m_operatorName=="Range")   doApply<0,0,0,1,0,RANGE>(state,particleIndices,selected);
    } else {
        if(m_operatorName=="Greater") doApply<0,0,0,0,1,GREATER>(state,particleIndices,selected);
        if(m_operatorName=="Less")    doApply<0,0,0,0,1,LESS>(state,particleIndices,selected);
        if(m_operatorName=="Equal")   doApply<0,0,0,0,1,EQUAL>(state,particleIndices,selected);
        if(m_operatorName=="Range")   doApply<0,0,0,0,1,RANGE>(state,particleIndices,selected);
    }
}

QString SelectionModifier::propertyName() const
{
    return m_propertyName;
}

float SelectionModifier::min() const
{
    return m_min;
}

float SelectionModifier::max() const
{
    return m_max;
}

QString SelectionModifier::operatorName() const
{
    return m_operatorName;
}

void SelectionModifier::setPropertyName(QString propertyName)
{
    if (m_propertyName == propertyName)
        return;

    m_propertyName = propertyName;
    emit propertyNameChanged(propertyName);
}

void SelectionModifier::setMin(float min)
{
    if (m_min == min)
        return;

    m_min = min;
    emit minChanged(min);
}

void SelectionModifier::setMax(float max)
{
    if (m_max == max)
        return;

    m_max = max;
    emit maxChanged(max);
}

void SelectionModifier::setOperatorName(QString operatorName)
{
    if (m_operatorName == operatorName)
        return;

    m_operatorName = operatorName;
    emit operatorNameChanged(operatorName);
}
