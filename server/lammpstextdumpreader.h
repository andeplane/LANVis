#ifndef LAMMPSTEXTDUMPREADER_H
#define LAMMPSTEXTDUMPREADER_H
#include "state.h"
#include <QObject>

class LAMMPSTextDumpReader : public QObject
{
    Q_OBJECT
public:
    explicit LAMMPSTextDumpReader(QObject *parent = 0);
    void readFile(QString fileName, QVector<class State *> &states);
signals:

public slots:
};

#endif // LAMMPSTEXTDUMPREADER_H
