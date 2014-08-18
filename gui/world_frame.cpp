#include "world_frame.h"

#include "../midi_engine.h"

#include <QPainter>
#include <QDebug>

WorldFrame::WorldFrame(QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Sunken);
}

void WorldFrame::drawWorld(QVector<int> world, int height, int width)
{
    world_info = world;
    world_height = height;
    world_width = width;
    update();
}

void WorldFrame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect world_rect = contentsRect();

    if (world_info.size() > 0) {
        double cell_size = world_height > world_width ?
                    (double)(world_rect.height()) / world_height :
                    (double)(world_rect.width()) / world_width;

        for (int h = 0; h < world_height; h++) {
            for (int w = 0; w < world_width; w++) {

                int value = world_info.at(h * world_width + w);

                if (value == world_state::EMPTY) {

                    double spread = cell_size/4;
                    double grass_height = cell_size/5;

                    QPoint bottom_center(w * cell_size + cell_size/2, (h + 1) * cell_size);
                    QPoint top_center = bottom_center - QPoint(0, grass_height);

                    painter.setPen(QColor(Qt::green));
                    painter.drawLine(bottom_center, top_center);
                    painter.drawLine(bottom_center, top_center - QPoint(spread, 0));
                    painter.drawLine(bottom_center, top_center + QPoint(spread, 0));
                    painter.setPen(QColor(Qt::black));

                }
                else if (value != world_state::SILENT) {

                    if (value != midi_state::PAUSE) painter.setBrush(QBrush(QColor(135, 206, 235, 200)));
                    else painter.setBrush(QBrush(QColor(255, 204, 0, 200)));

                    painter.drawRect(w * cell_size, h * cell_size, cell_size, cell_size);
                    if (value != midi_state::PAUSE) painter.drawText(w * cell_size, h * cell_size, cell_size, cell_size, Qt::AlignCenter, QString::number(value));

                } else {

                    painter.setBrush(QBrush(QColor(Qt::red)));
                    painter.drawRect(w * cell_size, h * cell_size, cell_size, cell_size);

                }

            }
        }

    }
}
