#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <math.h>
#include <QFontDatabase>
#include <QDesktopServices>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setValues();
    loadAll();

    ui->setupUi(this);

    showFullScreen();
    setWindowTitle("Bubbles");
    setCursor(Qt::BlankCursor);
    setMouseTracking(true);

    timerCreateId = startTimer(100);
    timerUpId = startTimer(15);
    timerDownId = startTimer(50);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::touch(QRect one, QRect two)
{
    int x1 = one.x() + one.width()/2;
    int y1 = one.y() + one.height()/2;

    int x2 = two.x() + two.width()/2;
    int y2 = two.y() + two.height()/2;

    int dX = x1-x2;
    int dY = y1-y2;

    int d = sqrt((dX*dX)+(dY*dY));

    if(d <= one.width()/2 + two.width()/2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(QRect(0, 0, width(), height()), backgrounds[currentBackgroundNum]);

    p.setFont(siteFont);
    p.setPen(Qt::blue);

    p.setOpacity(0.2);

    p.drawText(QRect(p.font().pixelSize()*0.2, height()-p.font().pixelSize()*1.2, width(), p.font().pixelSize()*2), Qt::AlignLeft, "CoolONE.ru");

    p.drawText(QRect(0, 0, width()-15, p.font().pixelSize()*2), Qt::AlignRight, "Bubbles v0.3");

    for(int f = 0; f<bubbles.size(); f++)
    {
        int x  = bubbles[f].rect.x()-bubbles[f].rect.width()/2;
        int y =  bubbles[f].rect.y()-bubbles[f].rect.height()/2;
        int size = bubbles[f].rect.width();
        p.drawImage(QRect(x ,y,size , size), bubbleImg);
    }
}

void Widget::setValues()
{
    baseDir = QApplication::applicationDirPath();

    currentBackgroundNum = 2;

    maxRandSize = width()/15;
}

void Widget::loadAll()
{
    for(int f = 0; f<backgroundsNum; f++)
    {
        backgrounds[f].load(":/images/background"+QString::number(f+1)+".jpg");
    }

    bubbleImg.load(":/images/bubble.png");

    siteFont.setFamily(QFontDatabase::applicationFontFamilies( QFontDatabase::addApplicationFont(":/fonts/arista/arista.ttf")).first() );
    siteFont.setPixelSize((width()+height())/2/10);
}

void Widget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerUpId) // ------------------- Timer Up -------------------
    {
        QVector<Bubble>::iterator iterUp;

        iterUp = bubbles.begin();
        while( iterUp != bubbles.end() )
        {
            if(iterUp->go)
            {
                int x = iterUp->rect.x()-1+rand()%3;
                int y = iterUp->rect.y() - iterUp->speed;
                int size = iterUp->rect.width();
                iterUp->rect.setRect(x, y, size, size);
            }

            if(iterUp->rect.y() < -iterUp->rect.height()/2)
            {
                iterUp = bubbles.erase(iterUp);
            }
            else
            {
                iterUp++;
            }
        }

        QVector<Bubble>::iterator iter1;
        iter1 = bubbles.begin();
        while( iter1 != bubbles.end() )
        {
            QVector<Bubble>::iterator iter2;

                iter2 = iter1;
                iter2++;
                if(iter1->go && iter2->go)
                {
                while( iter2 != bubbles.end() )
                {
                    if(touch(iter1->rect,iter2->rect) &&
                            iter1->rect.width() < maxSize &&
                            iter2->rect.width() < maxSize)
                    {
                        int x = (iter1->rect.x()+iter2->rect.x())/2;
                        int y = (iter1->rect.y()+iter2->rect.y())/2;
                        int size;

                        if(iter1->rect.width()/2 > iter2->rect.width()/2)
                            size = iter1->rect.width() + iter2->rect.width()/2;
                        else
                            size = iter1->rect.width()/2 + iter2->rect.width();

                        if(size>maxSize)
                            size = maxSize-1;

                        iter1->rect.setRect(x,y,size,size);
                        iter1->endSize = size;
                        iter1->speed = (rand()%(randSpeed-1))+4;
                        iter2 = bubbles.erase(iter2);
                    }
                    else
                    {
                        iter2++;
                    }
                }
            }

            iter1++;
        }
    }
    else if(event->timerId() == timerDownId) // ------------------- Timer Down -------------------
    {
        for(int f = 0; f<bubbles.size(); f++)
        {
            if(!bubbles[f].go)
            {
                if(bubbles[f].rect.width() <= bubbles[f].endSize && bubbles[f].rect.height() <= bubbles[f].endSize)
                {
                    int x = bubbles[f].rect.x();
                    int y = bubbles[f].rect.y()-1;
                    int size = bubbles[f].rect.width()+2;
                    bubbles[f].rect.setRect(x, y, size, size);
                }
                else
                {
                    bubbles[f].go = true;
                }
            }
        }
    }
    else if(event->timerId() == timerCreateId) // ------------------- Timer Create -------------------
    {
        createBubble();
    }
    update();
}

void Widget::createBubble()
{
    Bubble bub;
    bub.go = false;
    bub.endSize = rand()%maxRandSize;
    bub.speed = (rand()%(randSpeed-1))+4;
    int x = (bubbles[bubbles.size()-1].endSize / 2) + rand()%width() - bubbles[bubbles.size()-1].endSize;
    int y = height();
    int size = 1;
    bub.rect.setRect(x, y, size, size);

    bubbles.push_back(bub);
}

void Widget::createBubble(int x, int y)
{
    Bubble bub;
    bub.go = false;
    bub.endSize = rand()%maxRandSize;
    bub.rect.setRect(x, y, 1, 1);
    bub.speed = (rand()%(randSpeed-1))+4;

    bubbles.push_back(bub);
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_PageUp)
    {
        upBackground();
    }
    else if(event->key() == Qt::Key_PageDown)
    {
        downBackground();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    moveX = event->x();
    moveY = event->y();

    createBubble(moveX, moveY);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    releaseX = event->x();
    releaseY = event->y();

    if(event->x()<siteFont.pixelSize()*5 &&
            event->y()>height()-siteFont.pixelSize()*1)
    {
        // Open Site
        QDesktopServices::openUrl(QUrl("http://coolone.ru/"));
    }

    if(releaseX == clickX)
    {
        if(releaseX > width()/2)
        {
            upBackground();
        }
        else
        {
            downBackground();
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    clickX = event->x();
    clickY = event->y();
}

void Widget::upBackground()
{
    // Up
    if(currentBackgroundNum+1 < backgroundsNum)
    {
        currentBackgroundNum++;
    }
    else
    {
        currentBackgroundNum = 0;
    }
    update();
}

void Widget::downBackground()
{
    if(currentBackgroundNum>0)
    {
        currentBackgroundNum--;
    }
    else
    {
        currentBackgroundNum = backgroundsNum-1;
    }
    update();
}
