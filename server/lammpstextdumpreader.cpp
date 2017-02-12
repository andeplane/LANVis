#include "lammpstextdumpreader.h"
#include "particlestyle.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QElapsedTimer>

LAMMPSTextDumpReader::LAMMPSTextDumpReader(QObject *parent) : QObject(parent)
{

}

void LAMMPSTextDumpReader::readFile(QString fileName, QVector<State*> &states)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Could not open file " << fileName;
        return;
    }
    State *currentState = nullptr;
    QTextStream stream(&file);
    int timestep = -1;
    QElapsedTimer t;
    t.start();
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if(line.startsWith("ITEM: TIMESTEP")) {
            QString nextLine = stream.readLine();
            if(sscanf(nextLine.toUtf8().constData(), "%i", &timestep) != 1) {
                qDebug() << "Error, could not find timestep in line ";
                qDebug() << line;
                exit(1);
            }
            // if(currentState) return;
            currentState = new State();
            states.push_back(currentState);
            qDebug() << "Found timestep " << timestep;
        } else if(line.startsWith("ITEM: NUMBER OF ATOMS")) {
            unsigned int u;
            QString nextLine = stream.readLine();
            if(sscanf(nextLine.toUtf8().constData(), "%u", &u) != 1 || u > 1e9) {
                qDebug() << "Error, could not find number of atoms in line ";
                qDebug() << line;
                exit(1);
            }
            currentState->setNumberOfParticles(u);
            qDebug() << "Found number of particles " << u;
        } else if(line.startsWith("ITEM: BOX BOUNDS")) {
            QStringList xInfo = stream.readLine().split(" ", QString::SkipEmptyParts);
            QStringList yInfo = stream.readLine().split(" ", QString::SkipEmptyParts);
            QStringList zInfo = stream.readLine().split(" ", QString::SkipEmptyParts);
            if(xInfo.size() > 2 || yInfo.size() > 2 || zInfo.size() > 2) {
                qDebug() << "Error, triclinic boxes are not yet supported. Aborting!";
                exit(0);
            }
            bool ok;
            float xMin = xInfo.at(0).toFloat(&ok);
            float xMax = xInfo.at(1).toFloat(&ok);
            float yMin = yInfo.at(0).toFloat(&ok);
            float yMax = yInfo.at(1).toFloat(&ok);
            float zMin = zInfo.at(0).toFloat(&ok);
            float zMax = zInfo.at(1).toFloat(&ok);
            currentState->setOrigo(QVector3D(xMin, yMin, zMax));
            currentState->setSize(QVector3D(xMax-xMin, yMax-yMin, zMax-zMin));
        } else if(line.startsWith("ITEM: ATOMS")) {
            // For now, we assume the columns are as follows:
            // ITEM: ATOMS id type x y z
            // stream.readLine();
        } else {
            QStringList words = line.split(' ', QString::SkipEmptyParts);
            bool ok;
            int id = words[0].toInt(&ok);
            int type = words[1].toInt(&ok);
            float x = words[2].toDouble(&ok);
            float y = words[3].toDouble(&ok);
            float z = words[4].toDouble(&ok);

            QMap<QString, ParticleStyle *> &styles = currentState->particleStyles();
            QVector3D color(1.0, 0.8, 0.5);
            float radius = 1.0;
            QString typeString = QString("%1").arg(type);

            if(styles.contains(typeString)) {
                radius = styles[typeString]->radius;
                QColor qcolor = styles[typeString]->color;
                color[0] = qcolor.redF();
                color[1] = qcolor.greenF();
                color[2] = qcolor.blueF();
            }

            bool doShow = false;
            if(type==8||type==9) {
                doShow = false;
            }

            if(type!=8&&type!=9) {
                radius *= 0.4;
            }

            if(x>40 && x<160 && y>40 && y<160 && z > 160 && (type==1 || type==3)) {
                doShow = true;
            }
            if(type!=8 && type!=9 && (type==1 || type==3)) {
                doShow = true;
            } else {
                doShow = false;
            }

            if(doShow) {
                currentState->addParticle(QVector3D(x,y,z),color,radius);
            }
        }
    }

    qDebug() << "Read file using " << t.elapsed() << " ms.";
}
