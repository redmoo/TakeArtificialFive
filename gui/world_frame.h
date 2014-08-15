#ifndef WORLDFRAME_H
#define WORLDFRAME_H

#include <QFrame>

namespace world_state
{
    const int EMPTY = -2;
    const int SILENT = -1;
}

class WorldFrame : public QFrame
{
    Q_OBJECT

public:
    WorldFrame(QWidget *parent = 0);

    void drawWorld(QVector<int> world, int height, int width);

protected:
    void paintEvent(QPaintEvent *);

private:
    QVector<int> world_info;
    int world_height;
    int world_width;

};

#endif // WORLDFRAME_H
