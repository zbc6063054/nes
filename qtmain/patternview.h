#ifndef PATTENVIEW_H
#define PATTENVIEW_H

#include "../global.h"
#include <QWidget>
#include <QImage>

class QImage;

class PatternView : public QWidget
{
    Q_OBJECT
public:
    explicit PatternView(QWidget *parent = 0);

private:
    QImage patternImage;
    u8 patternAttr;

public:
    static PatternView *currentView;
private:
    void drawToImage(QImage &image);

protected:
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    
signals:
    
public slots:
    
};

#endif // PATTENVIEW_H
