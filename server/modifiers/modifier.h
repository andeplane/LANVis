#ifndef MODIFIER_H
#define MODIFIER_H

#include <QObject>
#include <vector>

class Modifier : public QObject
{
    Q_OBJECT
public:
    explicit Modifier(QObject *parent = 0);
    // void apply(std::vector<int> &particleIndices, std::vector<bool> &selected) = 0;
signals:

public slots:
};

#endif // MODIFIER_H
