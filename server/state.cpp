#include "state.h"
#include "particlestyle.h"
#include <QDebug>

State::State(QObject *parent) : QObject(parent),
    m_nx(0), m_ny(0), m_nz(0), m_chunkSize(0)
{
    setDefaultStyles();
}


void State::setDefaultStyles() {
    m_particleStyles.insert("hydrogen", new ParticleStyle(1.20, "#CCCCCC"));
    m_particleStyles.insert("helium", new ParticleStyle(1.40, "#D9FFFF"));
    m_particleStyles.insert("lithium", new ParticleStyle(1.82, "#CC80FF"));
    m_particleStyles.insert("beryllium", new ParticleStyle(1.53, "#C2FF00"));
    m_particleStyles.insert("boron", new ParticleStyle(1.92, "#FFB5B5"));
    m_particleStyles.insert("carbon", new ParticleStyle(1.70, "#505050"));
    m_particleStyles.insert("nitrogen", new ParticleStyle(1.55, "#3050F8"));
    m_particleStyles.insert("oxygen", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("fluorine", new ParticleStyle(1.35, "#90E050"));
    m_particleStyles.insert("neon", new ParticleStyle(1.54, "#3050F8"));
    m_particleStyles.insert("sodium", new ParticleStyle(2.27, "#AB5CF2"));
    m_particleStyles.insert("magnesium", new ParticleStyle(1.73, "#8AFF00"));
    m_particleStyles.insert("aluminium", new ParticleStyle(1.84, "#BFA6A6"));
    m_particleStyles.insert("silicon", new ParticleStyle(2.27, "#F0C8A0"));
    m_particleStyles.insert("phosphorus", new ParticleStyle(1.80, "#FF8000"));
    m_particleStyles.insert("sulfur", new ParticleStyle(1.80, "#FFFF30"));
    m_particleStyles.insert("chlorine", new ParticleStyle(1.75, "#1FF01F"));
    m_particleStyles.insert("argon", new ParticleStyle(1.88, "#80D1E3"));
    m_particleStyles.insert("potassium", new ParticleStyle(2.75, "#8F40D4"));
    m_particleStyles.insert("calcium", new ParticleStyle(2.31, "#3DFF00"));

    m_particleStyles.insert("H", new ParticleStyle(1.20, "#CCCCCC"));
    m_particleStyles.insert("He", new ParticleStyle(1.40, "#D9FFFF"));
    m_particleStyles.insert("Li", new ParticleStyle(1.82, "#CC80FF"));
    m_particleStyles.insert("Be", new ParticleStyle(1.53, "#C2FF00"));
    m_particleStyles.insert("B", new ParticleStyle(1.92, "#FFB5B5"));
    m_particleStyles.insert("C", new ParticleStyle(1.70, "#505050"));
    m_particleStyles.insert("N", new ParticleStyle(1.55, "#3050F8"));
    m_particleStyles.insert("O", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("F", new ParticleStyle(1.35, "#90E050"));
    m_particleStyles.insert("Ne", new ParticleStyle(1.54, "#3050F8"));
    m_particleStyles.insert("Na", new ParticleStyle(2.27, "#AB5CF2"));
    m_particleStyles.insert("Mg", new ParticleStyle(1.73, "#8AFF00"));
    m_particleStyles.insert("Al", new ParticleStyle(1.84, "#BFA6A6"));
    m_particleStyles.insert("Si", new ParticleStyle(2.27, "#F0C8A0"));
    m_particleStyles.insert("P", new ParticleStyle(1.80, "#FF8000"));
    m_particleStyles.insert("S", new ParticleStyle(1.80, "#FFFF30"));
    m_particleStyles.insert("Cl", new ParticleStyle(1.75, "#1FF01F"));
    m_particleStyles.insert("Ar", new ParticleStyle(1.88, "#80D1E3"));
    m_particleStyles.insert("K", new ParticleStyle(2.75, "#8F40D4"));
    m_particleStyles.insert("Ca", new ParticleStyle(2.31, "#3DFF00"));
    m_particleStyles.insert("Mo", new ParticleStyle(2.31, "#3DFF00"));

    m_particleStyles.insert("1", m_particleStyles["O"]);
    m_particleStyles.insert("2", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("3", m_particleStyles["C"]);
    m_particleStyles.insert("4", new ParticleStyle(1.84, "#119900"));
    m_particleStyles.insert("6", new ParticleStyle(1.84, "#119900"));

    m_particleStyles.insert("5", new ParticleStyle(1.70, "#505050"));
    m_particleStyles.insert("7", new ParticleStyle(1.52, "#AA0000"));
    m_particleStyles.insert("8", m_particleStyles["Mo"]);
    m_particleStyles.insert("9", m_particleStyles["S"]);
}


State::State(const State &state)
{
    qDebug() << "Error, should not call this...";
}

State::~State()
{
    m_chunkPtrs.clear();
    m_chunks.clear();
    m_allParticles.clear();
}

const std::vector<Particle> &State::allParticles() const
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

void State::setParticleStyles(const QMap<QString, ParticleStyle *> &particleStyles)
{
    m_particleStyles = particleStyles;
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

QMap<QString, ParticleStyle *> &State::particleStyles()
{
    return m_particleStyles;
}

void State::placeParticlesInChunks(float chunkSize, int lodLevels)
{
    m_chunkSize = chunkSize;
    setupChunks();
    float oneOverChunkSize = 1.0/m_chunkSize;
    for(Chunk &chunk : m_chunks) {
        chunk.reset();
    }

    for(Particle &particle : m_allParticles) {
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
        chunk.particles(0).push_back(particle);
    }
    int numParticles = 0;
    for(Chunk &chunk : m_chunks) {
        numParticles += chunk.particles(0).size();
    }
    qDebug() << "We got " << numParticles << " particles in this state";
    qDebug() << "All is " << m_allParticles.size() << " particles in this state";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0, 1);
    qDebug() << "Building LOD with " << lodLevels << " levels";
    for(Chunk &chunk : m_chunks) {
        chunk.buildLOD(lodLevels, generator, distribution);
    }
}

void State::addParticles(const std::vector<QVector3D> &positions, const std::vector<int> types, QVector3D origo, QVector3D size)
{
    m_allParticles.resize(positions.size());
    for(size_t particleIndex=0; particleIndex<positions.size(); particleIndex++) {
        const QVector3D &position = positions.at(particleIndex);
        float radius = 1.0;
        QVector3D color(1.0, 0.9, 0.8);
        QString typeAsString = QString("%1").arg(types[particleIndex]);
        if(m_particleStyles.contains(typeAsString)) {
            radius = m_particleStyles[typeAsString]->radius;
            color[0] = m_particleStyles[typeAsString]->color.redF();
            color[1] = m_particleStyles[typeAsString]->color.greenF();
            color[2] = m_particleStyles[typeAsString]->color.blueF();
        }

        m_allParticles[particleIndex].color = color;
        m_allParticles[particleIndex].radius = radius;
        m_allParticles[particleIndex].position = position;
    }
    m_origo = origo;
    m_size = size;
}

void State::addParticle(QVector3D position, int type)
{
    QVector3D color(1.0, 0.0, 0.0);
    float radius = 1.0;
    QString typeKey = QString("%1").arg(type);
    if(m_particleStyles.contains(typeKey)) {
        const QColor &qcolor = m_particleStyles[typeKey]->color;
        color[0] = qcolor.redF();
        color[1] = qcolor.greenF();
        color[2] = qcolor.blueF();
        radius = m_particleStyles[typeKey]->radius;
    }

    m_allParticles.push_back(Particle(position, color, radius));
}

void State::addParticle(QVector3D position,QVector3D color, float radius)
{
    m_allParticles.push_back(Particle(position, color, radius));
}

void State::setNumberOfParticles(int numberOfParticles)
{
    m_allParticles.reserve(numberOfParticles);
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

void State::setupChunks()
{
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

