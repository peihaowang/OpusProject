#include <QPainter>
#include <QTimer>
#include <QMediaContent>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QPaintEvent>
#include <iostream>
#include <random>
#include <math.h>

#include "PerlinNoise.hpp"
#include "ScribbleArea.h"

ScribbleArea::ScribbleArea(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , m_penWidth(3.0)
    , m_penColor(Qt::white)
    , m_penOpacity(1.0)
    , m_speed(1)
    , m_frameID(0)
    , m_paused(true)
    , m_offsetX(0)
    , m_image(100, 100)
{
//    m_image.setDevicePixelRatio(2.0);
    m_image.fill(Qt::black);

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, this, &ScribbleArea::onFrame);
    m_timer->setInterval(30);
    m_timer->start();

    m_player = new QMediaPlayer(this);
    m_player->setMedia(QMediaContent(QUrl("qrc:/sounds/opus.mp3")));
    m_player->setVolume(0);
    QObject::connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &ScribbleArea::onMediaStatusChanged);

    reset();

//    setMouseTracking(true);
//    setAttribute(Qt::WA_AcceptTouchEvents);
//    setAttribute(Qt::WA_StaticContents);

    this->setAcceptTouchEvents(true);
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

void ScribbleArea::onFrame()
{
    if(m_touchPoints.isEmpty()){
        int v = m_player->volume() - 5;
        if(v < 0) v = 0;
        m_player->setVolume(v);
    }else{
        if(!m_paused){
            int v = m_player->volume() + 25;
            if(v > 100) v = 100;
            m_player->setVolume(v);
        }
    }

    if(m_player->volume() <= 20){
        if(m_player->state() != QMediaPlayer::PausedState){
            m_player->pause();
        }
    }else{
        if(m_player->state() != QMediaPlayer::PlayingState){
            m_player->play();
        }
    }

    if(m_paused){
        m_touchPoints.clear();
        return;
    }

    animatePen();

    m_offsetX += m_speed;

    for(QPoint& pos : m_touchPoints){
//        QPoint lastPos = pos;
        pos += QPoint(m_speed, 0);
//        drawLineTo(lastPos, pos);
        drawPattern(pos);
    }

    if(m_offsetX >= m_image.width() / 2){
        m_image = m_image.copy(m_image.width() / 2, 0, m_image.width() / 2, m_image.height());
        resizeImage(m_image, QSize(m_image.width() * 2, m_image.height()));
        for(QPoint& pos : m_touchPoints) pos -= QPoint(m_offsetX, 0);
        m_offsetX = 0;
    }

    this->update();

    m_frameID++;
}

void ScribbleArea::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::EndOfMedia){
        m_player->stop();
        m_player->play();
        reset();
    }
}

void ScribbleArea::animatePen()
{
    const int step = 3;
    static bool firstIn = true;

    if(m_gradients.isEmpty()) return;

    ScribbleArea::Gradient g = m_gradients.head();
    /* if(m_player->duration() >= g.m_tStart && firstIn){
        changePen(g.m_cStart, g.m_wStart, 1.0);
        m_speed = g.m_pStart;
        firstIn = false;
    }else */

    if(g.m_tStart <= m_player->position() && m_player->position() < g.m_tEnd){
        if(m_frameID % step == 0){
            int dr = g.m_cEnd.red() - g.m_cStart.red();
            int dg = g.m_cEnd.green() - g.m_cStart.green();
            int db = g.m_cEnd.blue() - g.m_cStart.blue();

            int n = static_cast<int>(std::ceil((g.m_tEnd - g.m_tStart) / m_timer->interval()));

            QColor c = m_penColor;
            {
                int r = c.red() + (dr/n) * step;
                int g = c.green() + (dg/n) * step;
                int b = c.blue() + (db/n) * step;

                if(r < 0) { r = 0; }
                if(r > 255) { r = 255; }
                if(g < 0) { g = 0; }
                if(g > 255) { g = 255; }
                if(b < 0) { b = 0; }
                if(b > 255) { b = 255; }

                c.setRgb(r, g, b);
            }

            double dw = g.m_wEnd - g.m_wStart;
            double w = m_penWidth + (dw/n) * step;

            changePen(c, w, 1.0);

            m_speed += ((g.m_pEnd - g.m_pStart)/n) * step;
        }
    }else if(m_player->position() >= g.m_tEnd){
        changePen(g.m_cEnd, g.m_wEnd, 1.0);
        m_speed = g.m_pEnd;
        m_gradients.dequeue();
        firstIn = true;
    }
}

void ScribbleArea::reset()
{
    changePen(Qt::red, 5.0, 1.0);   // Initial Pen
    m_gradients.clear();
    m_gradients << Gradient(3500, QColor(Qt::red), 5.0, 8.0, 4500, QColor(Qt::blue), 2.0, 4.0)
                << Gradient(17000, QColor(Qt::blue), 2.0, 4.0, 18000, QColor(Qt::green), 4.0, 8.0)
                << Gradient(25000, QColor(Qt::green), 4.0, 8.0, 26000, QColor(Qt::magenta), 4.0, 9.0)
                << Gradient(37000, QColor(Qt::magenta), 4.0, 9.0, 40000, QColor(Qt::yellow), 2.0, 10.0)
                   ;
    m_speed = 8.0;

}

void ScribbleArea::restart()
{
    ScribbleArea::clear();
    m_player->stop();
    m_player->play();
    reset();
}

void ScribbleArea::stop()
{
    ScribbleArea::clear();
    m_player->stop();
    m_paused = true;
    reset();
}

void ScribbleArea::clear()
{
    m_image.fill(Qt::black);
}


#if 0
bool ScribbleArea::event(QEvent* event)
 {
     switch (event->type()) {
         case QEvent::TouchBegin:
         case QEvent::TouchUpdate:
         case QEvent::TouchEnd:
         {
             m_touchPoints.clear();

             QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
             for(const QTouchEvent::TouchPoint& touchPoint : touchPoints) {
//                 m_penColor = m_penColors.at(touchPoint.id() % m_penColors.count());
//                 m_penWidth = qreal(20) * touchPoint.pressure();
//                 QSizeF sz = touchPoint.ellipseDiameters();
//                 m_penWidth = std::max(sz.width(), sz.height());

//                 drawLineTo(tr(touchPoint.lastPos()), tr(touchPoint.pos()));
                 drawPattern(tr(touchPoint.pos()));

                 if(touchPoint.state() != Qt::TouchPointReleased){
                     m_touchPoints << tr(touchPoint.pos());
                 }
             }
             break;
         }
         default:
         {
             return this->event(event);
         }
     }
     return true;
 }
#endif


void ScribbleArea::touchEvent(QTouchEvent *event)
{
    m_touchPoints.clear();

    QList<QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    for(const QTouchEvent::TouchPoint& touchPoint : touchPoints) {
        drawPattern(tr(touchPoint.pos()));

        if(touchPoint.state() != Qt::TouchPointReleased){
            m_touchPoints << tr(touchPoint.pos());
        }
    }
}

#if not defined(Q_OS_IOS)

void ScribbleArea::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_touchPoints << tr(event->pos());
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && !m_touchPoints.isEmpty()) {
        drawPattern(tr(event->pos()));
        m_touchPoints[0] = tr(event->pos());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_touchPoints.isEmpty()) {
        drawPattern(tr(event->pos()));

        m_touchPoints.clear();
    }
}

#endif

#if 0
void ScribbleArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    QRect imageRect = tr(dirtyRect);
//    imageRect = QRect(imageRect.x() * 2, imageRect.y() * 2, imageRect.width() * 2, imageRect.height() * 2);
//    m_image.setDevicePixelRatio(2.0);
    painter.drawPixmap(dirtyRect, m_image, imageRect);
}

void ScribbleArea::resizeEvent(QResizeEvent* event)
{
    if (this->width() > m_image.width() || this->height() > m_image.height()) {
        int newWidth = qMax(static_cast<int>(this->width() * 2), m_image.width());
        int newHeight = qMax(static_cast<int>(this->height()), m_image.height());
        resizeImage(m_image, QSize(newWidth, newHeight));
        update();
    }
    this->resizeEvent(event);
}

#else

void ScribbleArea::paint(QPainter* painter)
{
    QRect dirtyRect = this->contentsBoundingRect().toRect();
    QRect imageRect = tr(dirtyRect);
    painter->drawPixmap(dirtyRect, m_image, imageRect);
}

void ScribbleArea::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    if(newGeometry.size() != oldGeometry.size()){
        if (this->width() > m_image.width() || this->height() > m_image.height()) {
            int newWidth = qMax(static_cast<int>(this->width() * 2), m_image.width());
            int newHeight = qMax(static_cast<int>(this->height()), m_image.height());
            resizeImage(m_image, QSize(newWidth, newHeight));
            update();
        }
    }
}

#endif

void ScribbleArea::drawLineTo(const QPoint& startPoint, const QPoint& endPoint)
{
    QPainter painter(&m_image);
//    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);

//    int rad = static_cast<int>((m_penWidth / 2) + 2);
//    QWidget::update(QRect(startPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void ScribbleArea::drawPattern(const QPoint& point)
{
    if(m_paused) return;

    QPainter painter(&m_image);

    double rot = std::rand() % 360;
    QTransform t; t.translate(point.x(), point.y()).rotate(rot).translate(-point.x(), -point.y());
    painter.setTransform(t);
    painter.setOpacity(m_penOpacity);

    QPoint pos(point.x() - m_penPattern.width() / 4, point.y() - m_penPattern.height() / 4);
    painter.drawPixmap(pos, m_penPattern);

//    QWidget::update(QRect(pos, m_penPattern.size() / 2));
}

void ScribbleArea::resizeImage(QPixmap& image, const QSize& newSize)
{
    if (image.size() == newSize) return;

    QPixmap newImage(newSize);
//    newImage.setDevicePixelRatio(2.0);
    newImage.fill(Qt::black);
    QPainter painter(&newImage);
    painter.drawPixmap(QPoint(0, 0), image);
    image = newImage;
}

void ScribbleArea::changePen(const QColor& c, double size, qreal opacity)
{
    m_penColor = c;
    m_penWidth = size;
    m_penOpacity = opacity;

    double ratio = size / 3.0;
    QImage img(":/images/stroke.png");
    img = img.scaled(ratio * img.size());
    img = img.alphaChannel();
    for(int i = 0; i < img.colorCount(); ++i) {
        QColor c0(c.rgb());
        c0.setAlpha(qGray(img.color(i)));
        img.setColor(i, c0.rgba());
    }

    m_penPattern = QPixmap::fromImage(img);
    m_penPattern.setDevicePixelRatio(2.0);

}
