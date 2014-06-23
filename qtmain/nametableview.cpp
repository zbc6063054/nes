#include "nametableview.h"
#include <QPainter>
#include "../core/nes.h"
#include "../core/ppu.h"

#define IMAGE_WIDTH     256
#define IMAGE_HEIGHT    240
#define SCALE 1

extern Nes nes;
NameTableView* NameTableView::currentView = NULL;

NameTableView::NameTableView(QWidget *parent) :
    QWidget(parent), ntImage(IMAGE_WIDTH, IMAGE_WIDTH, QImage::Format_RGB32)
{
    if(currentView == NULL){
        currentView = this;
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshImage()));
    timer->start(250);
    setFixedSize(IMAGE_WIDTH*SCALE*2, IMAGE_HEIGHT*SCALE*2);
}

void NameTableView::paintEvent(QPaintEvent *){
    QPainter painter;
    painter.begin(this);

#if 1
    for(int i=0; i<4; ++i){
        nes.ppu->drawNameTable((u32 *)ntImage.bits(), i);
        painter.drawImage(QRect(i%2*IMAGE_WIDTH*SCALE, i/2*IMAGE_HEIGHT*SCALE, IMAGE_WIDTH*SCALE, IMAGE_HEIGHT*SCALE),
                          ntImage, QRect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT));
    }
#else
    nes.ppu->tileRenderFrame((u32 *)ntImage.bits());
    painter.drawImage(QRect(0, 0, IMAGE_WIDTH*2*SCALE, IMAGE_HEIGHT*2*SCALE),
                      ntImage, QRect(0, 0, IMAGE_WIDTH*2, IMAGE_HEIGHT*2));
#endif
    painter.end();

}

void NameTableView::refreshImage(){
    repaint();
}

void NameTableView::closeEvent(QCloseEvent *){
    if(currentView == this){
        currentView = NULL;
    }
}
