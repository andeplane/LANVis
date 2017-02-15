#ifndef MODIFIER_H
#define MODIFIER_H

#include <QObject>
#include <vector>
#include "../client/particle.h"
#include "state.h"

class Modifier : public QObject
{
    Q_OBJECT
public:
    explicit Modifier(QObject *parent = 0);
    virtual void apply(const State &state, std::vector<int> &particleIndices, std::vector<bool> &selected, std::vector<ColoredParticle> &particles);
    virtual void apply(const State &state, std::vector<int> &particleIndices, std::vector<bool> &selected);
signals:

public slots:
};

#endif // MODIFIER_H
