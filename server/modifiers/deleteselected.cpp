#include "deleteselected.h"

DeleteSelected::DeleteSelected()
{

}


void DeleteSelected::apply(const State &, std::vector<int> &particleIndices, std::vector<bool> &selected)
{
    size_t keepCount = 0;
    for(int i=0; i<particleIndices.size(); i++) {
        if(!selected[i]) {
            particleIndices[keepCount++] = i;
        }
    }
    particleIndices.resize(keepCount);
    selected.clear();
    selected.resize(keepCount, 0);
}
