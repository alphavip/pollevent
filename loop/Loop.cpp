#include "Config.h"

#include "Poll.h"
#include "Loop.h"

bool Loop::Init() {
    pe = new PollEvent();
    pe->Init();
    return true;
}

void Loop::UnInit() {
    if(pe != nullptr)
        delete pe;
}

void Loop::Run() {
    run = true;
    while(run) {
        pe->Process();
    }
}
