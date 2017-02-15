#ifndef SELECTIONMODIFIER_H
#define SELECTIONMODIFIER_H

#include "modifier.h"

class SelectionModifier : public Modifier
{
    Q_OBJECT
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName NOTIFY propertyNameChanged)
    Q_PROPERTY(QString operatorName READ operatorName WRITE setOperatorName NOTIFY operatorNameChanged)
    Q_PROPERTY(float min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(float max READ max WRITE setMax NOTIFY maxChanged)
public:
    explicit SelectionModifier(Modifier *parent = nullptr);
    void apply(const State &state, std::vector<int> &particleIndices, std::vector<bool> &selected) override;
    QString propertyName() const;
    float min() const;
    float max() const;
    QString operatorName() const;

public slots:
    void setPropertyName(QString propertyName);
    void setMin(float min);
    void setMax(float max);
    void setOperatorName(QString operatorName);

signals:
    void propertyNameChanged(QString propertyName);
    void minChanged(float min);
    void maxChanged(float max);
    void operatorNameChanged(QString operatorName);

private:
    enum OperatorType { GREATER, LESS, EQUAL, RANGE } OperatorType;

    QString m_propertyName;
    template<int x, int y, int z, int type, int property, int operatorType>
    void doApply(const State &state, std::vector<int> &particleIndices, std::vector<bool> &selected);
    QString m_operatorName;
    float m_min;
    float m_max;
};

#endif // SELECTIONMODIFIER_H
