//
// Created by vsensu on 2020/7/19.
//

#ifndef NODECRAFT_RENDERWIDGET_H
#define NODECRAFT_RENDERWIDGET_H

#include <QtWidgets/QWidget>
#include <QPainter>
#include <QTimer>

#include "entt.hpp"
#include "components.h"

class RenderWidget : public QWidget
{
    Q_OBJECT
public:
    using QWidget::QWidget;

    void SetRegistry(entt::registry &registry)
    {
        this->registry = &registry;
        connect(&render_timer_, SIGNAL(timeout()), this, SLOT(update()));
        render_timer_.start(16);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        // draw rect nodes
        registry->view<position, RectNode>().each([&painter](auto &pos, auto &rect_node) {
            // gets all the components of the view at once ...
            QRectF rect(QPointF(pos.x, pos.y), QSizeF(rect_node.w, rect_node.h));
            painter.fillRect(rect, QBrush(QColor(rect_node.r, rect_node.g, rect_node.b)));
            // ...
        });

        // draw fps
        QFont font = painter.font();
        font.setPixelSize(24);
        painter.setFont(font);
        auto pen = painter.pen();
        pen.setColor(Qt::darkGreen);
        painter.setPen(pen);
        registry->view<FPS>().each([&painter, this](auto &c) {
            auto str = QString::number(c.value);
            QRect t_rect = painter.fontMetrics().boundingRect(str);
            t_rect.moveTopRight(QPoint(this->rect().width(), 0));
            painter.drawText(t_rect, str);
        });
    }

private:
    entt::registry *registry;
    QTimer render_timer_;
};

#endif //NODECRAFT_RENDERWIDGET_H
