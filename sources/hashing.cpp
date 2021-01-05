#include "hashing.h"
#include <QCryptographicHash>

Hashing::Hashing() {}

QByteArray Hashing::hash_sha256(QString input)
{
    QByteArray inputBArr(input.toStdString().c_str());
    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(inputBArr);
    QByteArray hashedInput = hasher.result().toHex();
    return hashedInput;
}
