#ifndef SPRITEVIEW_H
#define SPRITEVIEW_H

#include <QImage>
#include <QWidget>
#include <QTimer>

class SpriteView : public QWidget
{
    Q_OBJECT
public:
    explicit SpriteView(QWidget *parent = 0);

private:
    QImage spriteImage;
    QTimer *timer;

public:
    static SpriteView *currentView;
    
private:
    void drawToImage(QImage &image);

protected:
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *);
signals:
    
public slots:
    void refreshImage();
};

#endif // SPRITEVIEW_H
