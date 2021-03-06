#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>
#include <QDebug>
#include "../utils.h"

class DoubleSlider : public QSlider
{
    Q_OBJECT
public:
    DoubleSlider(QWidget * parent = 0) : QSlider(parent),
        m_dblMin(minimum()),
        m_dblMax(maximum())
    {

        connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
    }
    void setScale(double dblMin, double dblMax)
    {
        m_dblMin = dblMin; m_dblMax = dblMax;
    }


signals:
    void doubleValueChanged(double dblValue);

public slots:
    void notifyValueChanged(int nValue){
        emit doubleValueChanged(scaling(minimum(), maximum(), m_dblMin, m_dblMax ,nValue));
    }

private:
    double m_dblMin;
    double m_dblMax;

};

#endif // DOUBLESLIDER_H
