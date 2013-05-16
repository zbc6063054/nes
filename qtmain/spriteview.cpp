#include "spriteview.h"
#include <QPainter>
#include "../core/nes.h"
#include "../core/ppu.h"

SpriteView* SpriteView::currentView = NULL;
extern Nes nes;

#define VIEW_WIDTH 64
#define VIEW_HEIGHT 64
#define SCALE 2

SpriteView::SpriteView(QWidget *parent) :
    QWidget(parent), spriteImage(VIEW_WIDTH, VIEW_HEIGHT, QImage::Format_RGB32)
{
    if(currentView == NULL){
        currentView = this;
    }
    setWindowTitle(tr("Sprite View"));
    setFixedSize(VIEW_WIDTH*SCALE, VIEW_HEIGHT*SCALE);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshImage()));
    timer->setInterval(200);
    timer->start();
}

void SpriteView::closeEvent(QCloseEvent *){
    if(currentView == this){
        currentView = NULL;
    }
}

void SpriteView::paintEvent(QPaintEvent *){
    QPainter painter;
    painter.begin(this);
    drawToImage(spriteImage);
    painter.drawImage(QRect(0, 0, VIEW_WIDTH*SCALE, VIEW_HEIGHT*SCALE),
                      spriteImage, QRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT));

    painter.end();
}

void SpriteView::drawToImage(QImage &image){
    nes.ppu->drawSprite((u32 *)image.bits());
}

void SpriteView::refreshImage(){
    repaint();
}
