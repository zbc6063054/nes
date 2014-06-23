#include <QPainter>
#include "patternview.h"
#include "../core/nes.h"
#include "../core/ppu.h"
#include "../core/log.h"

#define PATTERN_WIDTH 8
#define PATTERN_HEIGHT 8

#define PATTERN_SCALE 2
#define PATTERN_SCALE 2

extern Nes nes;

PatternView* PatternView::currentView = NULL;

PatternView::PatternView(QWidget *parent) :
    QWidget(parent), patternImage(PATTERN_WIDTH*16, PATTERN_HEIGHT*16, QImage::Format_RGB32)
{
    if(currentView == NULL){
        currentView = this;
    }
    patternAttr = 0;
    setFixedSize(PATTERN_WIDTH*16*PATTERN_SCALE, PATTERN_HEIGHT*16*PATTERN_SCALE*2);
    setWindowTitle(tr("Pattern View"));
}

void PatternView::drawToImage(QImage &image){
}

void PatternView::paintEvent(QPaintEvent *event){
    QPainter painter;
    painter.begin(this);
    nes.ppu->drawPattern(patternAttr, (u32 *)patternImage.bits(), 0x00);
    painter.drawImage(QRect(0, 0, PATTERN_WIDTH*16*PATTERN_SCALE, PATTERN_HEIGHT*16*PATTERN_SCALE),
                      patternImage, QRect(0, 0, patternImage.width(), patternImage.height()));

    nes.ppu->drawPattern(patternAttr, (u32 *)patternImage.bits(), 0x1000);
    painter.drawImage(QRect(0, PATTERN_HEIGHT*16*PATTERN_SCALE,
                            PATTERN_WIDTH*16*PATTERN_SCALE, PATTERN_HEIGHT*16*PATTERN_SCALE),
                      patternImage, QRect(0, 0, patternImage.width(), patternImage.height()));
    painter.end();
}

void PatternView::closeEvent(QCloseEvent *){
    if(currentView == this){
        currentView = NULL;
    }
}

void PatternView::mouseDoubleClickEvent(QMouseEvent *){
    patternAttr++;
    if(patternAttr == 4){
        patternAttr = 0;
    }
    drawToImage(patternImage);
    repaint();
}
