#include "spherespointgeometry.h"
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/QBufferDataGenerator>
#include <Qt3DRender/qattribute.h>
#include <qmath.h>
#include <QVector3D>

using namespace Qt3DRender;

class SpheresPointVertexDataFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
    SpheresPointVertexDataFunctor()
    {
    }

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        const int verticesCount = 1;
        // vec3 pos
        const quint32 vertexSize = 3 * sizeof(float);

        QByteArray verticesData;
        verticesData.resize(vertexSize*verticesCount);
        float *verticesPtr = reinterpret_cast<float*>(verticesData.data());

        // Vertex 1
        *verticesPtr++ = 0.0;
        *verticesPtr++ = 0.0;
        *verticesPtr++ = 0.0;

        return verticesData;
    }

    bool operator ==(const QBufferDataGenerator &other) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(other);
        return true;
    }

    QT3D_FUNCTOR(SpheresPointVertexDataFunctor)
};

//class SpheresPointIndexDataFunctor : public QBufferDataGenerator
//{
//public:
//    SpheresPointIndexDataFunctor()
//    {
//    }

//    QByteArray operator ()() Q_DECL_OVERRIDE
//    {
//        const int indexCount = 1;
//        const int indexSize = sizeof(quint16);
//        QByteArray indicesBytes;
//        indicesBytes.resize(indexCount * indexSize);
//        quint16 *indicesPtr = reinterpret_cast<quint16*>(indicesBytes.data());
//        *indicesPtr++ = 0;
//        return indicesBytes;
//    }

//    bool operator ==(const QBufferDataGenerator &other) const Q_DECL_OVERRIDE
//    {
//        Q_UNUSED(other);
//        return true;
//    }

//    QT3D_FUNCTOR(SpheresPointIndexDataFunctor)
//};

void SpheresPointGeometry::init()
{
    m_positionAttribute = new QAttribute(this);
    // m_idAttribute = new QAttribute(this);
    // m_indexAttribute = new QAttribute(this);

    m_vertexBuffer = new QBuffer(QBuffer::VertexBuffer, this);
    // m_indexBuffer = new QBuffer(QBuffer::IndexBuffer, this);

    const quint32 elementSize = 3;
    const quint32 stride = elementSize * sizeof(float);
    const int vertexCount = 1;
    const int indexCount = 1;

    m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setVertexBaseType(QAttribute::Float);
    m_positionAttribute->setVertexSize(3);
    m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexBuffer);
    m_positionAttribute->setByteStride(stride);
    m_positionAttribute->setCount(vertexCount);

//    m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
//    m_indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);
//    m_indexAttribute->setBuffer(m_indexBuffer);
//    m_indexAttribute->setCount(indexCount);

    m_vertexBuffer->setDataGenerator(QBufferDataGeneratorPtr(new SpheresPointVertexDataFunctor()));
    // m_indexBuffer->setDataGenerator(QBufferDataGeneratorPtr(new SpheresPointIndexDataFunctor()));

    addAttribute(m_positionAttribute);
    // addAttribute(m_idAttribute);
    // addAttribute(m_indexAttribute);
}

SpheresPointGeometry::SpheresPointGeometry(Qt3DCore::QNode *parent)
    : QGeometry(parent)
{
    init();
}

void SpheresPointGeometry::updateVertices()
{
}

/*!
 * Updates the indices based on rings and slices.
 */
void SpheresPointGeometry::updateIndices()
{
}
