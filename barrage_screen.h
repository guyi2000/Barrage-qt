#ifndef BARRAGE_SCREEN_H
#define BARRAGE_SCREEN_H

#include <QWidget>
#include "barrage_item.h"
#include <QList>

class XYBarrageScreen : public QWidget
{
    Q_OBJECT
public:
    ~XYBarrageScreen();
    static XYBarrageScreen *getScreen();
    bool forceTop();

public slots:
    void insertItem(XYBarrageItem *item, int pos = 0); // 字幕插队
    void addItem(XYBarrageItem *item);
    void delItem(XYBarrageItem *item);
    void setMaxBarrageNumber(int max);
    void changeForceTop();
    void setForceTop(bool top);

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    void paintLineContents(QPainter &painter,
                           XYBarrageItem *item,
                           XYContents *lineContents,
                           QPoint &currentPos,
                           qreal lineHeight,
                           qreal offsetX,
                           qreal offsetY);
    bool rectCovered(const QRect &rect, const QList<QRect> &rects);
    explicit XYBarrageScreen(QWidget *parent = 0);

private:
    QList<XYBarrageItem *> mlistBarrageItems;
    static XYBarrageScreen *mopInstance;

    int                   miMaxBarrageNumber;
    int                   miRefreshTimer;
    bool                  mbForceTop;
};

#endif // BARRAGE_SCREEN_H
