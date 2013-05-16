
#include <QApplication>
#include "window.h"
#include <string.h>
#include "../core/nes.h"
#include "../core/cpu.h"
#include "../core/memory.h"
#include "../core/threadutil.h"
#include "../core/debug.h"
#include "../core/test.h"

Nes nes;

void SystemInit(){

    nes.init();
//    nes.loadFile("mario.nes");
//    nes.loadFile("test123234.nes");
    nes.loadFile("wings.nes");
    nes.reset();
    nes.start();

}

int main(int argc, char *argv[])
{
#ifdef WIN32
    LOGI("it's windows system!\n");
#endif
    SystemInit();
    test();
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
