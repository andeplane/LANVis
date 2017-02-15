#include "modifier.h"

Modifier::Modifier(QObject *parent) : QObject(parent)
{

}

void Modifier::apply(const State &, std::vector<int> &, std::vector<bool> &, std::vector<ColoredParticle> &)
{

}

void Modifier::apply(const State &, std::vector<int> &, std::vector<bool> &)
{

}
