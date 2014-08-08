#ifndef CORE_H
#define CORE_H

#include <QObject>

class Core : public QObject
{
    Q_OBJECT

public:
    explicit Core(QObject *parent = 0);

public:
    void initialize();

private:


};

#endif // CORE_H
