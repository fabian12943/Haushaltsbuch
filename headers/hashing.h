#ifndef HASHING_H
#define HASHING_H

#include <QString>

class Hashing
{
public:
    Hashing();
    static QByteArray hash_sha256(QString input);
};

#endif // HASHING_H
