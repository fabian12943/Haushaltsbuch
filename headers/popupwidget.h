#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>

class PopUpWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit PopUpWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void setPopupText(const QString& text);
    void show();

private:
    QLabel label;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;
};

#endif // POPUPWIDGET_H
