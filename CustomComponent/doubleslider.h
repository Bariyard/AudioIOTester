#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>
#include <QDebug>

class DoubleSlider : public QSlider
{
    Q_OBJECT
public:
    DoubleSlider(QWidget * parent = 0) : QSlider(parent),
        dblMin(minimum()),
        dblMax(maximum())
    {

        connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
    }
    void setScale(double min, double max)
    {
        dblMin = min; dblMax = max;
    }


signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value){
        auto scaling = [](double old_min, double old_max, double new_min, double new_max, double old_value)
        {
            return (new_max - new_min) / (old_max - old_min) * (old_value - old_min) + new_min;
        };

        emit doubleValueChanged(scaling(minimum(), maximum(), dblMin,dblMax ,value));
    }

private:
    double dblMin;
    double dblMax;

};

#endif // DOUBLESLIDER_H
