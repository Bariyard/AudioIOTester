#ifndef SEEKER_H
#define SEEKER_H

#include <QWidget>

class Seeker : public QWidget
{
    Q_OBJECT
public:
    Seeker(QWidget *parent = 0);
    ~Seeker();

    void put_CurrentNumFrame(unsigned long numFrame);
    void put_Incrementer(double dblIncrementer);

signals:

protected:
    void paintEvent(QPaintEvent *event);

private:
    unsigned long   m_nCurrentFrame;
    double          m_dblIncrementer;

};

#endif // SEEKER_H
