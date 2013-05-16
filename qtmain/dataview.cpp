#include "dataview.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTextStream>

#define NUM_ROW 4

DataView::DataView(QWidget *parent) :
    QWidget(parent)
{
    data = NULL;
    size = 0;
    startAddr = 0;
}

void DataView::paintEvent(QPaintEvent *event){
    QPainter painter;
    painter.begin(this);

    painter.fillRect(event->rect(), QBrush(Qt::black));

    if(data != NULL){
        QString buffer;
        QTextStream out(&buffer);
        out.setIntegerBase(16);
        out.setPadChar(QChar('0'));
        out.setNumberFlags(QTextStream::UppercaseDigits);

        out.setFieldWidth(4);
        out << startAddr ;
        out.setFieldWidth(0);
        out << ": ";
        for(u16 i=0; i < NUM_ROW*16; ++i){
            out.setFieldWidth(2);
            out << *(data + startAddr + i);
            if((i+1)%16 == 0 && (i+1)!=NUM_ROW*16){
                out.setFieldWidth(0);
                out << "\n" ;
                out.setFieldWidth(4);
                out << startAddr + i + 1;
                out.setFieldWidth(0);
                out<< ":";
            }
            out.setFieldWidth(0);
            if((i+1)%16 == 7)
                out << "~";
            else
                out << " ";
        }

        painter.setPen(QPen(Qt::white));
        painter.drawText(event->rect(), buffer);
    }else{
        painter.fillRect(event->rect(), QBrush(Qt::white));
    }

    painter.end();
}

void DataView::setData(u8 *data_new, int size){
    data = data_new;
    this->size = size;
    repaint();
}

void DataView::setStartAddr(u16 addr){
    this->startAddr = addr;
    repaint();
}
