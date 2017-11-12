#include "RenderSystem/RenderSystem.h"

int main() {

    RenderSystem * rs = new RenderSystem();
    rs->run();
    delete rs;

    return 0;
}