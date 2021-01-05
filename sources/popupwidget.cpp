#include "popupwidget.h"
#include <QPainter>

PopUpWidget::PopUpWidget(QWidget *parent):
    QWidget(parent)
    {
        resize(200, 42);

        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_ShowWithoutActivating);

        animation.setTargetObject(this);
        animation.setPropertyName("popupOpacity");
        animation.setDuration(150);

        label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        layout.addWidget(&label, 0, 0);
        setLayout(&layout);
    }

void PopUpWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    // Draw the popup here
    // You can always pick an image and use drawPixmap to
    // draw it in order to make things simpler

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Prepare the popup dimensions
    QRect roundedRectDimensions;
    roundedRectDimensions.setX(rect().x() + 5);
    roundedRectDimensions.setY(rect().y() + 5);
    roundedRectDimensions.setWidth(rect().width() - 10);
    roundedRectDimensions.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(Qt::lightGray));

    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidth(3);
    painter.setPen(pen);

    // Draw the popup body
    painter.drawRoundedRect(roundedRectDimensions, 15, 15);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    // Draw the popup pointer
    const QPointF points[3] = { QPoint(roundedRectDimensions.x(), roundedRectDimensions.height() / 2 - 5 + 3),
        QPoint(roundedRectDimensions.x(), roundedRectDimensions.height() / 2 + 5 + 3),
        QPoint(roundedRectDimensions.x() - 5, roundedRectDimensions.height() / 2 + 3)
    };

    painter.drawPolygon(points, 3);
}

void PopUpWidget::setPopupText(const QString &text)
{
    label.setText(text);
}

void PopUpWidget::show()
{
    setWindowOpacity(0.0);

    animation.setStartValue(0.0);
    animation.setEndValue(1.0);

    QWidget::show();

    animation.start();
}

void PopUpWidget::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float PopUpWidget::getPopupOpacity() const
{
    return popupOpacity;
}
