#include "atomdata.h"
AtomData::AtomData(QObject *parent) : QObject(parent)
{

}

void AtomData::resize(int size)
{
    positions.resize(size);
    colors.resize(size);
    radii.resize(size);
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
}

long AtomData::memoryUsage()
{
    return (positions.capacity() + colors.capacity())*sizeof(QVector3D)
            +(radii.capacity())*sizeof(float);
}

AtomData::~AtomData()
{
    reset();
}
