/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include "widget.h"

#include "../core/nes.h"

#define SCREEN_WIDTH (256)
#define SCREEN_HEIGHT (240)

#define DATA_WIDTH (256+16)
#define DATA_HEIGHT (240)


#define SCALE 2

extern Nes nes;

//! [0]
Widget::Widget(QWidget *parent)
    : QWidget(parent), screenImage(DATA_WIDTH, DATA_HEIGHT, QImage::Format_RGB32)
{
    elapsed = 0;
    resize(SCREEN_WIDTH*SCALE, SCREEN_HEIGHT*SCALE);
    setAttribute(Qt::WA_OpaquePaintEvent);
    connect(this, SIGNAL(refresh()), this, SLOT(animate()));
    nes.setRefreshFunc(notifyScreenRefresh, this);
}
//! [0]

//! [1]
void Widget::animate()
{
    repaint();
}
//! [1]

//! [2]
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
//    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    drawToImage(&screenImage);
    painter.drawImage(QRect(0, 0, this->width(), this->height()),
                      screenImage, QRect(8, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    painter.setPen(QPen(Qt::white));
    painter.drawText(20, 20, QString().sprintf("fps: %0.2f", nes.getFps()));
    painter.end();
}
//! [2]

void Widget::drawToImage(QImage *image){
    const u32 *data = nes.getScreenData();
    memcpy(image->bits(), data, DATA_WIDTH*DATA_HEIGHT*sizeof(u32));
}
