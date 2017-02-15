#include "state.h"
#include "particlestyle.h"
#include "modifiers/modifiers.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>

State::State(QObject *parent) : QObject(parent),
    m_nx(0), m_ny(0), m_nz(0), m_chunkSize(0)
{

}

State::State(const State &state)
{
    qDebug() << "Error, should not call this...";
}

State::~State()
{
    for(Chunk &chunk : m_chunks) {
        chunk.reset();
    }

    m_chunkPtrs.clear();
    m_chunks.clear();
    m_allParticles.clear();
}

const std::vector<IdentifiableParticle> &State::allParticles() const
{
    return m_allParticles;
}

void State::sortChunks(QVector3D point)
{
    std::sort(m_chunkPtrs.begin(), m_chunkPtrs.end(),
              [&](const Chunk* a, const Chunk* b)
    {
        float da = a->minDistanceTo(point);
        float db = b->minDistanceTo(point);
        return da < db;
    });
}

std::vector<Chunk *> State::chunkPtrs() const
{
    return m_chunkPtrs;
}

QVector3D State::origo() const
{
    return m_origo;
}

void State::setOrigo(const QVector3D &origo)
{
    m_origo = origo;
}

QVector3D State::size() const
{
    return m_size;
}

void State::setSize(const QVector3D &size)
{
    m_size = size;
}

void State::placeParticlesInChunks(ClientState &clientState)
{
    m_chunkSize = clientState.chunkSize();
    setupChunks();
    float oneOverChunkSize = 1.0/m_chunkSize;
    for(Chunk &chunk : m_chunks) {
        chunk.reset();
    }

    for(size_t particleIndex=0; particleIndex < m_allParticles.size(); particleIndex++) {
        IdentifiableParticle &particle = m_allParticles[particleIndex];

        // Apply periodic boundary conditions
        if(particle.position[0] < m_origo[0]) particle.position[0] += m_size[0];
        if(particle.position[1] < m_origo[1]) particle.position[1] += m_size[1];
        if(particle.position[2] < m_origo[2]) particle.position[2] += m_size[2];

        if(particle.position[0] >= m_origo[0]+m_size[0]) particle.position[0] -= m_size[0];
        if(particle.position[1] >= m_origo[1]+m_size[1]) particle.position[1] -= m_size[1];
        if(particle.position[2] >= m_origo[2]+m_size[2]) particle.position[2] -= m_size[2];

        int i = (particle.position[0]-m_origo[0]) * oneOverChunkSize;
        int j = (particle.position[1]-m_origo[1]) * oneOverChunkSize;
        int k = (particle.position[2]-m_origo[2]) * oneOverChunkSize;
        if(index(i,j,k) >= m_chunks.size()) {
            qDebug() << "Error, skipping particle outside simulation box.";
            continue;
        }
        Chunk &chunk = m_chunks[index(i,j,k)];
        chunk.particleIndices(0).push_back(particleIndex);
    }

    int numParticles = 0;
    for(Chunk &chunk : m_chunks) {
        numParticles += chunk.particleIndices(0).size();
    }
    qDebug() << "We got " << numParticles << " particles in this state";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0, 1);

    qDebug() << "Building LOD with " << clientState.lodLevels() << " levels";
    for(Chunk &chunk : m_chunks) {
        chunk.buildLOD(clientState, generator, distribution);
    }

    for(int lod=0; lod<clientState.lodLevels(); lod++) {
        int numParticles = 0;
        for(Chunk &chunk : m_chunks) {
            numParticles += chunk.particleIndices(lod).size();
        }
        qDebug() << "Found " << numParticles << " for lod level " << lod;
    }
    clientState.setChunksDirty(false);

    qDebug() << "Finished with LOD building.";
}

void State::addParticles(const std::vector<QVector3D> &positions, const std::vector<QString> types, QVector3D origo, QVector3D size)
{
    m_allParticles.resize(positions.size());
    for(size_t particleIndex=0; particleIndex<positions.size(); particleIndex++) {
        const QVector3D &position = positions.at(particleIndex);
        QString type = types[particleIndex];
        m_allParticles[particleIndex].position = position;
        m_allParticles[particleIndex].type = type;
        m_allParticles[particleIndex].index = particleIndex;
    }
    m_origo = origo;
    m_size = size;
}

void State::addParticle(QVector3D position, QString type, std::vector<float> particleProperties)
{
    int index = m_allParticles.size(); // The current size will be index of this particle
    m_allParticles.push_back(IdentifiableParticle(position, type, index));
    if(particleProperties.size() > 0) {
        m_particleProperties.push_back(particleProperties);
    }

}

void State::setNumberOfParticles(int numberOfParticles)
{
    m_selected.reserve(numberOfParticles);
    m_particleIndices.reserve(numberOfParticles);
    m_allParticles.reserve(numberOfParticles);
    if(m_particlePropertyNames.size() > 0) {
        m_particleProperties.reserve(numberOfParticles);
    }
}

void State::setParticlePropertyNames(QStringList particlePropertyNames)
{
    m_particlePropertyNames = particlePropertyNames;
}

void State::reset()
{
    for(Chunk &chunk : m_chunks) {
        chunk.reset();
    }
    m_allParticles.clear();
    m_nx = 0;
    m_ny = 0;
    m_nz = 0;
    m_chunkSize = 0;
    m_size = QVector3D();
    m_origo = QVector3D();
}

void State::applyModifiers(ClientState &state)
{
    QElapsedTimer t;
    t.start();
    SelectionModifier selection;
    selection.setPropertyName("x");
    selection.setMin(300);
    selection.setOperatorName("Greater");

    m_selected.clear();
    m_selected.resize(m_allParticles.size(), false);
    m_particleIndices.resize(m_allParticles.size());

    qDebug() << "Initial things: " << t.restart();
    std::iota (std::begin(m_particleIndices), std::end(m_particleIndices), 0); // Fill with 0, 1, ..., N-1.
    qDebug() << "Filling indices: " << t.restart();
    qDebug() << "I had " << m_particleIndices.size() << " particles.";
    selection.apply(*this, m_particleIndices, m_selected);
    qDebug() << "Applying: " << t.restart();

    DeleteSelected del;
    del.apply(*this, m_particleIndices, m_selected);
    qDebug() << "Deleting: " << t.restart();
    qDebug() << "Now I have " << m_particleIndices.size() << " particles.";
    state.setChunksDirty(true);
    state.setParticlesDirty(true);
}

void State::setupChunks()
{
    qDebug() << "Setting up chunks";
    m_nx = m_size[0]/m_chunkSize + 1;
    m_ny = m_size[1]/m_chunkSize + 1;
    m_nz = m_size[2]/m_chunkSize + 1;
    int numChunks = m_nx*m_ny*m_nz;
    m_chunks.resize(numChunks);
    m_chunkPtrs.resize(numChunks);
    for(int i=0; i<numChunks; i++) {
        m_chunkPtrs[i] = &m_chunks[i]; // TODO: handle this better than raw pointers
    }
    qDebug() << "Chunks setup with " << m_nx << " " << m_ny << " " << m_nz << " chunks";
    qDebug() << "Origo: " << m_origo << " and size: " << m_size;

    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                Chunk &chunk = m_chunks[index(i,j,k)];
                std::vector<QVector3D> &corners = chunk.corners();
                corners[0] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[1] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
                corners[2] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[3] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[4] = QVector3D(m_origo[0] + (i+0)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
                corners[5] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+0)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
                corners[6] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+0)*m_chunkSize);
                corners[7] = QVector3D(m_origo[0] + (i+1)*m_chunkSize, m_origo[1] + (j+1)*m_chunkSize, m_origo[2] + (k+1)*m_chunkSize);
            }
        }
    }
}

