#ifndef _SCRIBBLE_AREA_H_
#define _SCRIBBLE_AREA_H_

#include <QList>
#include <QColor>
#include <QPoint>
#include <QQueue>
#include <QPixmap>
#include <QTouchEvent>
#include <QMediaPlayer>
#include <QQuickPaintedItem>

class QMouseEvent;
class QTouchEvent;

class QMediaPlayer;

class ScribbleArea : public QQuickPaintedItem
{

    Q_OBJECT

public:

    ScribbleArea(QQuickItem* parent = nullptr);

    // QWidget interface
protected:

    inline QPoint tr(const QPoint& p) const { return p + QPoint(m_offsetX, 0); }
    inline QPoint tr(const QPointF& p) const { return p.toPoint() + QPoint(m_offsetX, 0); }
    inline QRect tr(const QRect& r) const { return r.adjusted(m_offsetX, 0, m_offsetX, 0); }

//    bool event(QEvent* event);

    void touchEvent(QTouchEvent* event);

#if not defined(Q_OS_IOS)
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
#endif
//    void paintEvent(QPaintEvent* event);
    void paint(QPainter* painter);
//    void resizeEvent(QResizeEvent* event);
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    void drawLineTo(const QPoint& startPoint, const QPoint& endPoint);
    void drawPattern(const QPoint& point);
    void resizeImage(QPixmap& image, const QSize& newSize);

    void changePen(const QColor& c, double size, qreal opacity);

    void animatePen();
    void reset();

protected slots:

    void onFrame();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

public slots:

    void restart();
    void stop();
    void clear();

protected:

    QTimer*                         m_timer;

    double                          m_penWidth;
    QColor                          m_penColor;
    double                          m_penOpacity;
    QPixmap                         m_penPattern;

    int                             m_speed;
    int                             m_frameID;
    bool                            m_paused;
    int                             m_offsetX;

    QList<QPoint>                   m_touchPoints;

    QPixmap                         m_image;

    QMediaPlayer *                  m_player;

    struct Gradient{
        int         m_tStart;
        QColor      m_cStart;
        double      m_wStart;
        int         m_pStart;

        int         m_tEnd;
        QColor      m_cEnd;
        double      m_wEnd;
        int         m_pEnd;

        Gradient()
            : m_tStart(0), m_cStart(Qt::white), m_wStart(0), m_pStart(0)
        , m_tEnd(0), m_cEnd(Qt::white), m_wEnd(0), m_pEnd(0)
        {
            return;
        }

        Gradient(int tStart, const QColor& cStart, double wStart, int pStart
            , int tEnd, const QColor& cEnd, double wEnd, int pEnd)
        : m_tStart(tStart), m_cStart(cStart), m_wStart(wStart), m_pStart(pStart)
        , m_tEnd(tEnd), m_cEnd(cEnd), m_wEnd(wEnd), m_pEnd(pEnd)
        {
            return;
        }
    };

    QQueue<Gradient>                m_gradients;

public:

    Q_PROPERTY(bool paused READ isPaused WRITE setPaused NOTIFY pausedChanged)

    bool isPaused() const { return m_paused; }
    void setPaused(bool paused) { m_paused = paused; }

signals:

    void pausedChanged();

};

#endif // _SCRIBBLE_AREA_H_
