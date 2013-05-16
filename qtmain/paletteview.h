#ifndef PALETTEVIEW_H
#define PALETTEVIEW_H

#include <QWidget>
#include <QTimer>

class PaletteView : public QWidget
{
    Q_OBJECT
public:
    explicit PaletteView(QWidget *parent = 0);

private:
    bool directClose;
    QTimer *timer;

public:
    static PaletteView *currentView;

    
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *);

signals:
    
public slots:
    void refreshImage();
};

#endif // PALETTEVIEW_H
