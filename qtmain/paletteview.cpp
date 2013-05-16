#include <QPainter>
#include <QBrush>
#include <QRect>
#include <QColor>
#include "paletteview.h"
#include "../core/nes.h"
#include "../core/memory.h"

#define RECT_WIDTH 20
#define RECT_HEIGHT 20

extern Nes nes;

PaletteView* PaletteView::currentView = NULL;

PaletteView::PaletteView(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(RECT_WIDTH*16, RECT_HEIGHT*2);
    setWindowTitle(tr("palette view"));

    directClose = false;
    currentView = this;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshImage()));
    timer->start(500);
}

void PaletteView::paintEvent(QPaintEvent *event){
    QPainter painter;
    painter.begin(this);

    u8* data = bg_pattle;
    for(int i=0; i<16; ++i){
        QRectF rect(i*RECT_WIDTH, 0, RECT_WIDTH, RECT_HEIGHT);
        QColor color(nes.getColor(data[i]));
        painter.fillRect(rect, color);
    }
    data = spr_pattle;
    for(int i=0; i<16; ++i){
        QRectF rect(i*RECT_WIDTH, RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT);
        QColor color(nes.getColor(data[i]));
        painter.fillRect(rect, color);
    }

    painter.end();
}

void PaletteView::closeEvent(QCloseEvent *){
    if(currentView == this){
        currentView = NULL;
    }
}

void PaletteView::refreshImage(){
    repaint();
}
