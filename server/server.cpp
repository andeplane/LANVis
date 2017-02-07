#include "server.h"
#include "../client/xyzreader.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Server::Server() : m_rCut(50)
{
    setDefaultStyles();
}

void Server::setDefaultStyles() {
    m_atomStyles.insert("hydrogen", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyles.insert("helium", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyles.insert("lithium", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyles.insert("beryllium", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyles.insert("boron", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyles.insert("carbon", new AtomStyle(1.70, "#505050"));
    m_atomStyles.insert("nitrogen", new AtomStyle(1.55, "#3050F8"));
    m_atomStyles.insert("oxygen", new AtomStyle(1.52, "#AA0000"));
    m_atomStyles.insert("fluorine", new AtomStyle(1.35, "#90E050"));
    m_atomStyles.insert("neon", new AtomStyle(1.54, "#3050F8"));
    m_atomStyles.insert("sodium", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyles.insert("magnesium", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyles.insert("aluminium", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyles.insert("silicon", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("phosphorus", new AtomStyle(1.80, "#FF8000"));
    m_atomStyles.insert("sulfur", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyles.insert("chlorine", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyles.insert("argon", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyles.insert("potassium", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyles.insert("calcium", new AtomStyle(2.31, "#3DFF00"));

    m_atomStyles.insert("H", new AtomStyle(1.20, "#CCCCCC"));
    m_atomStyles.insert("He", new AtomStyle(1.40, "#D9FFFF"));
    m_atomStyles.insert("Li", new AtomStyle(1.82, "#CC80FF"));
    m_atomStyles.insert("Be", new AtomStyle(1.53, "#C2FF00"));
    m_atomStyles.insert("B", new AtomStyle(1.92, "#FFB5B5"));
    m_atomStyles.insert("C", new AtomStyle(1.70, "#505050"));
    m_atomStyles.insert("N", new AtomStyle(1.55, "#3050F8"));
    m_atomStyles.insert("O", new AtomStyle(1.52, "#AA0000"));
    m_atomStyles.insert("F", new AtomStyle(1.35, "#90E050"));
    m_atomStyles.insert("Ne", new AtomStyle(1.54, "#3050F8"));
    m_atomStyles.insert("Na", new AtomStyle(2.27, "#AB5CF2"));
    m_atomStyles.insert("Mg", new AtomStyle(1.73, "#8AFF00"));
    m_atomStyles.insert("Al", new AtomStyle(1.84, "#BFA6A6"));
    m_atomStyles.insert("Si", new AtomStyle(2.27, "#F0C8A0"));
    m_atomStyles.insert("P", new AtomStyle(1.80, "#FF8000"));
    m_atomStyles.insert("S", new AtomStyle(1.80, "#FFFF30"));
    m_atomStyles.insert("Cl", new AtomStyle(1.75, "#1FF01F"));
    m_atomStyles.insert("Ar", new AtomStyle(1.88, "#80D1E3"));
    m_atomStyles.insert("K", new AtomStyle(2.75, "#8F40D4"));
    m_atomStyles.insert("Ca", new AtomStyle(2.31, "#3DFF00"));
}

void Server::loadXYZ(QString fileName)
{
    XYZReader reader;
    reader.readFile(fileName);
    const QVector<QVector3D> &positions = reader.positions();
    const QVector<QString>   &types     = reader.types();
    m_allParticles.resize(positions.size());

    for(int i=0; i<positions.size(); i++) {
        const QVector3D &position = positions.at(i);
        float radius = 1.0;
        QVector3D color(1.0, 0.9, 0.8);

        if(m_atomStyles.contains(types[i])) {
            radius = m_atomStyles[types[i]]->radius;
            color[0] = m_atomStyles[types[i]]->color.redF();
            color[1] = m_atomStyles[types[i]]->color.greenF();
            color[2] = m_atomStyles[types[i]]->color.blueF();
        }
        m_allParticles[i].color = color;
        m_allParticles[i].radius = radius;
        m_allParticles[i].position = position;
    }
}

void Server::updatePositions()
{
    m_subParticles.clear();
    float cutsq = m_rCut*m_rCut;
    for(const Particle &particle : m_allParticles) {
        float dx = particle.position[0] - m_cameraPosition[0];
        float dy = particle.position[1] - m_cameraPosition[1];
        float dz = particle.position[2] - m_cameraPosition[2];
        float dr2 = dx*dx + dy*dy + dz*dz;
        if(dr2 < cutsq) {
            m_subParticles.push_back(particle);
        }
    }
}

void Server::writePositions(QString fileName)
{
    int numBytes = m_subParticles.size()*sizeof(Particle);
    const char *array = reinterpret_cast<const char*>(&m_subParticles.front());

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Could not open file " << fileName;
    }
    file.write(array, numBytes);
    file.close();
}

void Server::update(QString clientStateFileName)
{
    QFile loadFile(clientStateFileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open state file");
        return;
    }

    QByteArray stateData = loadFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(stateData));
    QJsonObject   obj = doc.object();
    QJsonArray    arr = obj["cameraPosition"].toArray();

    m_cameraPosition[0] = arr[0].toDouble();
    m_cameraPosition[1] = arr[1].toDouble();
    m_cameraPosition[2] = arr[2].toDouble();
    updatePositions();
}
