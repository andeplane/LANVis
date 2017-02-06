#include "atomdata.h"
AtomData::AtomData(QObject *parent) : QObject(parent)
{

}

void AtomData::resize(int size)
{
    positions.resize(size);
    colors.resize(size);
    radii.resize(size);
    types.resize(size);
    visible.resize(size);
}

int AtomData::size()
{
    return positions.size();
}

void AtomData::reset()
{
    positions.clear();
    colors.clear();
    radii.clear();
    types.clear();
    visible.clear();
}

long AtomData::memoryUsage()
{
    return (positions.capacity() + colors.capacity())*sizeof(QVector3D)
            +(radii.capacity())*sizeof(float)
            +types.capacity()*sizeof(int)
            +visible.capacity()*sizeof(bool);
}

AtomData::~AtomData()
{
    reset();
}
