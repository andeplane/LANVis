#ifndef DELETESELECTED_H
#define DELETESELECTED_H

#include "modifier.h"

class DeleteSelected : public Modifier
{
    Q_OBJECT
public:
    DeleteSelected();
    // Modifier interface

public:
    void apply(const State &, std::vector<int> &particleIndices, std::vector<bool> &selected) override;
};

#endif // DELETESELECTED_H
