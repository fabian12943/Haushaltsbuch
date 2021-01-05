#ifndef NUMBERFORMATDELEGATE_H
#define NUMBERFORMATDELEGATE_H

#include <QStyledItemDelegate>
#include <math.h>


class NumberFormatDelegate : public QStyledItemDelegate{

    Q_OBJECT
    Q_DISABLE_COPY(NumberFormatDelegate)

public:
    NumberFormatDelegate(QObject *parent = Q_NULLPTR);
    QString displayText(const QVariant &value, const QLocale &locale) const Q_DECL_OVERRIDE{
        switch(value.type()){
        case QMetaType::Float:
            return locale.toString(value.toFloat(),'f', 2) + " €";
        case QMetaType::Double:
            return locale.toString(value.toDouble(),'f', 2) + " €";
        default:
            return QStyledItemDelegate::displayText(value,locale);
        }
    }
};

#endif // NUMBERFORMATDELEGATE_H
