#ifndef LAMMPSTEXTDUMPREADER_H
#define LAMMPSTEXTDUMPREADER_H

#include <QObject>

class LAMMPSTextDumpReader : public QObject
{
    Q_OBJECT
public:
    explicit LAMMPSTextDumpReader(QObject *parent = 0);

signals:

public slots:
};

#endif // LAMMPSTEXTDUMPREADER_H