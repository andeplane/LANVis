#include "particlesubset.h"
#include <QElapsedTimer>
#include <omp.h>
#include <QDebug>

ParticleSubset::ParticleSubset(QObject *parent) : QObject(parent), m_state(nullptr)
{
    setDefaultStyles();
}


void ParticleSubset::setDefaultStyles() {
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


std::vector<ColoredParticle> &ParticleSubset::particles()
{
    return m_particles;
}

QVector3D ParticleSubset::boundingBoxMin() const
{
    return m_boundingBoxMin;
}

QVector3D ParticleSubset::boundingBoxMax() const
{
    return m_boundingBoxMax;
}

//void ParticleSubset::insertion_sort(const ClientState &clientState){
//    for (int i = 0; i < m_particles.size(); i++){
//        qDebug() << "Particle " << i;
//        int j = i;

//        bool condition;
//        if(m_particles[j].color[3] == 1.0 && m_particles[j-1].color[3] == 1.0) {
//            condition = false;
//        } else {
//            condition = (m_particles[j].position - clientState.cameraPosition()).lengthSquared() > (m_particles[j-1].position - clientState.cameraPosition()).lengthSquared();
//        }

//        while (j > 0 && condition) {
//            std::swap(m_particles[j], m_particles[j-1]);
//            j--;

//            if(m_particles[j].color[3] == 1.0 && m_particles[j-1].color[3] == 1.0) {
//                condition = false;
//            } else {
//                condition = (m_particles[j].position - clientState.cameraPosition()).lengthSquared() > (m_particles[j-1].position - clientState.cameraPosition()).lengthSquared();
//            }
//        }
//    }
//}

void ParticleSubset::updatePositions(State &state, const ClientState &clientState)
{
    QElapsedTimer t;
    t.start();
    m_state = &state;
    qDebug() << "Clearing positions";
    m_particles.clear();
    int particleCount = 0;
    qDebug() << "Clearing positions took " << t.restart() << "ms. Now sorting chunks.";
    state.sortChunks(clientState.cameraPosition());
    qDebug() << "Sorting chunks took " << t.restart()<<". Now reserving space to indices.";

    m_boundingBoxMin = QVector3D(1e9,1e9,1e9);
    m_boundingBoxMax = QVector3D(-1e9,-1e9,-1e9);

    m_particleIndices.clear();
    m_particleIndices.reserve(clientState.maxNumberOfParticles());

    qDebug() << "Reserving indices took " << t.restart()<<". Now checking chunks.";

    std::vector<Chunk*> chunks;
    for(Chunk *chunk : state.chunkPtrs()) {
        float distance = chunk->minDistanceTo(clientState.cameraPosition());
        int lod = distance / clientState.lodDistance();
        if(lod > clientState.lodLevels()) lod = clientState.lodLevels();

        m_boundingBoxMin[0] = std::min(m_boundingBoxMin[0], chunk->corners()[0][0]);
        m_boundingBoxMin[1] = std::min(m_boundingBoxMin[1], chunk->corners()[0][1]);
        m_boundingBoxMin[2] = std::min(m_boundingBoxMin[2], chunk->corners()[0][2]);

        m_boundingBoxMax[0] = std::max(m_boundingBoxMax[0], chunk->corners()[7][0]);
        m_boundingBoxMax[1] = std::max(m_boundingBoxMax[1], chunk->corners()[7][1]);
        m_boundingBoxMax[2] = std::max(m_boundingBoxMax[2], chunk->corners()[7][2]);

        chunks.push_back(chunk);
        particleCount += chunk->particleIndices(lod).size();
        if(particleCount > clientState.maxNumberOfParticles()) break;
    }

    qDebug() << "Checking chunks took " << t.restart() << " ms. Got " << particleCount << " particles and " << chunks.size() << " chunks. Now sorting chunk particles.";
    if(clientState.sort()) {
#pragma omp parallel for num_threads(clientState.numThreads())
        for(size_t i=0; i<chunks.size(); i++) {
            Chunk *chunk = chunks[i];
            float distance = chunk->minDistanceTo(clientState.cameraPosition());
            int lod = distance / clientState.lodDistance();
            if(lod > clientState.lodLevels()) lod = clientState.lodLevels();
            chunk->sort(clientState.cameraPosition(), state.allParticles(), lod);
        }
    }

    qDebug() << "Sorting took " << t.restart() << "ms. Now copying particles.";
    for(size_t i=0; i<chunks.size(); i++) {
        Chunk *chunk = chunks[i];
        float distance = chunk->minDistanceTo(clientState.cameraPosition());
        int lod = distance / clientState.lodDistance();
        if(lod > clientState.lodLevels()) lod = clientState.lodLevels();
        m_particleIndices.insert( m_particleIndices.end(), chunk->particleIndices(lod).begin(), chunk->particleIndices(lod).end() );
    }
    qDebug() << "Checking chunks took " << t.restart() << " ms. Now resizing particle array.";

    m_particles.resize(m_particleIndices.size());
    qDebug() << "Resizing took " << t.restart() << " ms. Processing particles.";
    const std::vector<IdentifiableParticle> &particles = state.allParticles();
    const size_t numParticles = m_particles.size();


#pragma omp parallel for num_threads(clientState.numThreads())
    for(size_t i=0; i<numParticles; i++) {
        int particleIndex = m_particleIndices[i];
        const IdentifiableParticle &particle = particles[particleIndex];

        QColor color = "red";
        float radius = 1.0;
        if(m_particleStyles.contains(particle.type)) {
            color = m_particleStyles[particle.type]->color;
            radius = m_particleStyles[particle.type]->radius;
        }
        m_particles[i].color[0] = color.redF();
        m_particles[i].color[1] = color.greenF();
        m_particles[i].color[2] = color.blueF();
        m_particles[i].color[3] = color.alphaF();
        if( (particle.position - QVector3D(75,75,75)).lengthSquared() > 50*50) {
            m_particles[i].color[3] *= 0.05;
        }
        m_particles[i].position = particle.position;
        m_particles[i].radius = radius;

        if(!clientState.enableTransparency()) {
            m_particles[i].color[3] = 1.0;
        }
    }

    if(clientState.enableTransparency()) {
        qDebug() << "Processing finished after " << t.restart() << " ms. Now sorting based on alpha";
        std::sort(m_particles.begin(), m_particles.end(),
                  [&](ColoredParticle &a, ColoredParticle &b)
        {
            float da = (a.position - clientState.cameraPosition()).lengthSquared();
            float db = (b.position - clientState.cameraPosition()).lengthSquared();
            if(a.color[3] == 1.0 && b.color[3] == 1.0) {
                return da < db;
            }
            return da > db;
        });
        qDebug() << "Sorting took " << t.elapsed() << " ms.";
    }
}
