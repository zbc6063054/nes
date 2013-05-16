#ifndef NAMETABLEVIEW_H
#define NAMETABLEVIEW_H

#include <QWidget>
#include <QImage>
#include <QTimer>

class NameTableView : public QWidget
{
    Q_OBJECT
public:
    explicit NameTableView(QWidget *parent = 0);
    
private:
    QImage ntImage;
    QTimer *timer;
public:
    static NameTableView *currentView;

private:

protected:
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *);

signals:
    

public slots:
    void refreshImage();
};

#endif // NAMETABLEVIEW_H
