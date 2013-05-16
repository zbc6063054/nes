#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "../global.h"
#include <QWidget>

class DataView : public QWidget
{
    Q_OBJECT
public:
    explicit DataView(QWidget *parent = 0);

private:
    u8 *data;
    int size;
    u16 startAddr;

protected:
    void paintEvent(QPaintEvent *);

public:
    void setData(u8 *data_new, int size);
    void setStartAddr(u16 addr);

signals:
    
public slots:
    
};

#endif // DATAVIEW_H
