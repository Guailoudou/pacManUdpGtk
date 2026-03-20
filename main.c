#include "head.h"

int main(int argc, char *argv[]){
    createMap();
    cdrui_init(argc,argv);
}

void start()
{
    while(isrun)
    {
        tick();
    }
}