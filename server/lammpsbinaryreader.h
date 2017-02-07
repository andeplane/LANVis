#ifndef LAMMPSBINARYREADER_H
#define LAMMPSBINARYREADER_H

#include <QString>
#include <QVector>
#include <QVector3D>

#include "stdio.h"
#include "string.h"

// these must match settings in src/lmptype.h which builds LAMMPS with
//   -DLAMMPS_SMALLBIG (the default), -DLAMMPS_BIGBIG, or -DLAMMPS_SMALLSMALL
// you can edit the tools/Makefile to enforce the same setting
//   for the build of binary2txt, e.g.
//   g++ -g -DLAMMPS_BIGBIG binarytxt.o -o binary2txt
//   again -DLAMMPS_SMALLBIG is the default

#include "stdint.h"
#define __STDC_FORMAT_MACROS
#include "inttypes.h"

#ifndef PRId64
#define PRId64 "ld"
#endif

#if !defined(LAMMPS_SMALLSMALL) && !defined(LAMMPS_BIGBIG) && !defined(LAMMPS_SMALLBIG)
#define LAMMPS_SMALLBIG
#endif

#if defined(LAMMPS_SMALLBIG)
typedef int tagint;
typedef int64_t bigint;
#define BIGINT_FORMAT "%" PRId64
#elif defined(LAMMPS_SMALLSMALL)
typedef int tagint;
typedef int bigint;
#define BIGINT_FORMAT "%d"
#else /* LAMMPS_BIGBIG */
typedef int64_t tagint;
typedef int64_t bigint;
#define BIGINT_FORMAT "%" PRId64
#endif

class LAMMPSBinaryReader
{
public:
    LAMMPSBinaryReader() { }
    void readFile(QString fileName);
    const QVector<QVector3D> &positions() const;
    QVector<int> types() const;
    QVector3D origo() const;
    QVector3D size() const;

private:
    QVector3D m_origo;
    QVector3D m_size;
    QVector<QVector3D> m_positions;
    QVector<int> m_types;
};

#endif // LAMMPSBINARYREADER_H
