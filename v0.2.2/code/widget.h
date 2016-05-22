#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QRect>
#include <QPropertyAnimation>

namespace Ui {
class Widget;
}

class Bubble
{
public:
    QRect rect;
    int endSize;
    int speed;
    bool go;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    static const int maxSize = 100;
    static const int backgroundsNum = 5;
    static const int randSpeed = 5;

    int maxRandSize;

    int currentBackgroundNum;

    int timerDownId, timerUpId, timerCreateId;

    QImage backgrounds[backgroundsNum];
    QImage bubbleImg;
    QString baseDir;
    QFont siteFont;

    QVector<Bubble> bubbles;

    bool touch(QRect one, QRect two);

private slots:
    void paintEvent(QPaintEvent *);
    void setValues();
    void loadAll();
    void timerEvent(QTimerEvent *);
    void createBubble();
    void createBubble(int x, int y);
    void keyReleaseEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
