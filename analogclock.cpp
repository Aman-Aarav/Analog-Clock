#include "analogclock.h"

#include <QTime>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QtMath>
#include <QTimer>
#include <QPainter>



AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
    timer->start(1000);

    setWindowTitle("Analog Clock");
    resize(400, 400);

}

void AnalogClock::paintEvent(QPaintEvent *)
{
    QTime time = QTime::currentTime();
    qInfo()<< time.toString("hh:mm:ss");

    int side = qMin(width(), height());

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.translate(width() / 2, height() / 2); // shift at the center of the widget
    p.scale(side / 200.0, side / 200.0);

    //  1. gradient QGradient::AquaSplash preset Background
    QGradient glowGrad(QGradient::AquaSplash);
    p.setBrush(glowGrad);
    p.setPen(Qt::NoPen);
    p.drawEllipse(-100, -100, 200, 200);

    //  2. Metallic Silver Ring
    QLinearGradient ringGrad(-100, -100, 100, 100);
    ringGrad.setColorAt(0.0, QColor(230, 230, 230));
    ringGrad.setColorAt(0.5, QColor(180, 180, 180));
    ringGrad.setColorAt(1.0, QColor(250, 250, 250));
    p.setPen(QPen(QBrush(ringGrad), 5));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(-95, -95, 190, 190);

    // === 3. Silver Ticks ===
    QLinearGradient tickGrad(0, 0, 100, 0);
    tickGrad.setColorAt(0, QColor(255, 255, 255));
    tickGrad.setColorAt(1, QColor(180, 180, 180));
    p.setPen(QPen(QBrush(tickGrad), 2));
    for (int i = 0; i < 60; ++i) {
        // Default tick length
        int inner = 88;
        int outer = 94;
        int penWidth = 2;

        // Emphasize 0°, 45°, 75°, and 90° marks

        int angle = i * 6;  // Each tick = 6 degrees

        if (angle == 0 || angle == 90 || angle == 180 || angle == 270) {
            inner = 76;       // longer tick
            penWidth = 6;     // thicker tick


        } else if (i % 5 == 0) {
            inner = 80;       // regular hour tick
            penWidth = 3;
        }

        p.setPen(QPen(QBrush(tickGrad), penWidth));
        p.drawLine(inner, 0, outer, 0);
        p.rotate(6.0);
    }

    //  4. Hour Hand
    QLinearGradient hourGrad(0, -70, 0, 10);
    hourGrad.setColorAt(0, QColor(190, 220, 255));
    hourGrad.setColorAt(1, QColor(60, 90, 130));
    p.save();
    p.setBrush(hourGrad);
    p.setPen(Qt::NoPen);
    p.rotate(30.0 * (time.hour() + time.minute() / 60.0));
    QPoint hourHand[3] = {QPoint(6, 10), QPoint(-6, 10), QPoint(0, -50)};
    p.drawConvexPolygon(hourHand, 3);
    p.restore();

    //  5. Minute Hand
    QLinearGradient minuteGrad(0, -70, 0, 10);
    minuteGrad.setColorAt(0, QColor(190, 220, 255));
    minuteGrad.setColorAt(1, QColor(60, 90, 130));
    p.save();
    p.setBrush(minuteGrad);
    p.setPen(Qt::NoPen);
    p.rotate(6.0 * (time.minute() + time.second() / 60.0));
    QPoint minuteHand[3] = {QPoint(3, 10), QPoint(-3, 10), QPoint(0, -75)};
    p.drawConvexPolygon(minuteHand, 3);
    p.restore();

    //  6. Second Hand (Red Glow)
    p.save();
    p.rotate(6.0 * (time.second() + time.msec() / 1000.0));
    QLinearGradient secondGrad(0, -90, 0, 10);
    secondGrad.setColorAt(0, QColor(255, 100, 100));
    secondGrad.setColorAt(1, QColor(100, 0, 0));
    p.setBrush(secondGrad);
    p.setPen(Qt::NoPen);
    QPoint secondHand[3] = {QPoint(2, 10), QPoint(-2, 10), QPoint(0, -82)};
    p.drawConvexPolygon(secondHand, 3);
    p.setBrush(QColor(255, 50, 50, 120));
    p.drawEllipse(-4, -4, 8, 8);
    p.restore();

    //  7. Center  CAP
    QRadialGradient capGrad(0, 0, 8);
    capGrad.setColorAt(0, QColor(255, 255, 255));
    capGrad.setColorAt(1, QColor(160, 160, 160));
    p.setBrush(capGrad);
    p.setPen(Qt::NoPen);
    p.drawEllipse(-5, -5, 10, 10);

    //  8. Glass Reflection
    p.save();
    QRadialGradient glassGrad(0, -70, 120, 0, -70);
    glassGrad.setColorAt(0.0, QColor(255, 255, 255, 180)); // bright top reflection
    glassGrad.setColorAt(0.3, QColor(255, 255, 255, 60));
    glassGrad.setColorAt(0.7, QColor(255, 255, 255, 15));
    glassGrad.setColorAt(1.0, QColor(255, 255, 255, 0));
    p.setBrush(glassGrad);
    p.setPen(Qt::NoPen);
    p.drawEllipse(-100, -100, 200, 200);
    p.restore();

    //  9. Bottom Glow
    QRadialGradient bottomGlow(0, 60, 120, 0, 60);
    bottomGlow.setColorAt(0.0, QColor(255, 255, 255, 80));
    bottomGlow.setColorAt(1.0, Qt::transparent);
    p.setBrush(bottomGlow);
    p.drawEllipse(-100, -100, 200, 200);
}
