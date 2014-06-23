
#include <QtGui>
#include <QFileDialog>
#include <QMenuBar>
#include "widget.h"
#include "window.h"
#include "paletteview.h"
#include "../core/nes.h"
#include "../core/controller.h"
#include "debugdialog.h"
#include "patternview.h"
#include "spriteview.h"
#include "nametableview.h"

extern Nes nes;

struct {
    Qt::Key sys_key;
    Controller::Key game_key;
} keyMapper[] = {
    { Qt::Key_Up, Controller::KEY_UP } ,
    { Qt::Key_Down, Controller::KEY_DOWN } ,
    { Qt::Key_Left, Controller::KEY_LEFT } ,
    { Qt::Key_Right, Controller::KEY_RIGHT } ,
    { Qt::Key_Z, Controller::KEY_A } ,
    { Qt::Key_X, Controller::KEY_B } ,
    { Qt::Key_B, Controller::KEY_START } ,
    { Qt::Key_N, Controller::KEY_SELECT } ,
};

//! [0]
Window::Window()
    : QMainWindow()
{
    setWindowTitle(tr("ZNes"));
    resize(256*2, 240*2);

    createActions();
    createMenus();

    Widget *native = new Widget(this);
    this->setCentralWidget(native);
}
//! [0]

void Window::createActions(){
    mViewPalette = new QAction(tr("View &Palette"), this);
    mViewPalette->setStatusTip(tr("view nes palette"));
    mShowDebug = new QAction(tr("&Debug"), this);
    mShowDebug->setStatusTip(tr("show debug dialog"));
    mViewPattern = new QAction(tr("View Pattern"), this);
    mViewPalette->setStatusTip("view nes pattern");
    mViewSprite = new QAction(tr("View Sprite"), this);
    mViewSprite->setStatusTip(tr("view nes sprite"));
    mViewNameTable = new  QAction(tr("View NameTable"), this);
    mViewNameTable->setStatusTip(tr("view nes NameTable"));

    mFileOpen = new QAction(tr("&Open File"), this);
    mFileOpen->setStatusTip(tr("open nes file"));

    connect(mViewPalette, SIGNAL(triggered()), this, SLOT(viewPalette()));
    connect(mShowDebug, SIGNAL(triggered()), this, SLOT(showDebugDialog()));
    connect(mViewPattern, SIGNAL(triggered()), this, SLOT(viewPattern()));
    connect(mViewSprite, SIGNAL(triggered()), this, SLOT(viewSprite()));
    connect(mViewNameTable, SIGNAL(triggered()), this, SLOT(viewNameTable()));
    connect(mFileOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
}

void Window::viewPalette(){
    if(PaletteView::currentView == NULL){
        PaletteView *view = new PaletteView();
        view->show();
    }
}

void Window::showDebugDialog(){
    if(DebugDialog::currentDebugDialog == NULL){
        DebugDialog *dialog = new DebugDialog(this);
        dialog->show();
    }
}

void Window::viewPattern(){
    if(PatternView::currentView == NULL){
        PatternView *view = new PatternView();
        view->show();
    }
}

void Window::viewSprite(){
    if(SpriteView::currentView == NULL){
        SpriteView *view = new SpriteView();
        view->show();
    }
}

void Window::viewNameTable(){
    if(NameTableView::currentView == NULL){
        NameTableView *view = new NameTableView();
        view->show();
    }
}

void Window::fileOpen(){
    QString file = QFileDialog::getOpenFileName(this, tr("open nes file"),
                                 NULL, tr("nes file (*.nes)"));
    if( !file.isNull() ){
        nes.stop();
        nes.loadFile((const char *)file.toLocal8Bit());
        nes.reset();
        nes.start();
    }
}

void Window::createMenus(){
    mFile = menuBar()->addMenu(tr("&File"));
    mFile->addAction(mFileOpen);

    mView = menuBar()->addMenu(tr("&Debug"));
    mView->addAction(mViewPalette);
    mView->addAction(mShowDebug);
    mView->addAction(mViewPattern);
    mView->addAction(mViewSprite);
    mView->addAction(mViewNameTable);
}

void Window::keyPressEvent(QKeyEvent *event){
    int len = sizeof(keyMapper) / sizeof(keyMapper[0]);
    for(int i=0; i<len; i++){
        if(event->key() == keyMapper[i].sys_key)
            nes.control->setKeyState(keyMapper[i].game_key, true);
    }
}

void Window::keyReleaseEvent(QKeyEvent *event){
    int len = sizeof(keyMapper) / sizeof(keyMapper[0]);
    for(int i=0; i<len; i++){
        if(event->key() == keyMapper[i].sys_key)
            nes.control->setKeyState(keyMapper[i].game_key, false);
    }
    if(event->key() == Qt::Key_Space){
        if(nes.isRunning())
            nes.pause();
        else
            nes.resume();
    }
}

void Window::closeEvent(QCloseEvent *){
    nes.stop();
}
