#include "chunk.h"

float Chunk::minDistanceTo(const QVector3D &point) const
{
    float minDistance = corners[0].distanceToPoint(point);
    for(int cornerIndex=1; cornerIndex<8; cornerIndex++) {
        minDistance = std::min(minDistance, corners[cornerIndex].distanceToPoint(point));
    }

    return minDistance;
}
