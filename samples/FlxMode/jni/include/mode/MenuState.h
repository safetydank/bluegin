#pragma once

#include "flx/state.h"
#include "flx/emitter.h"
#include "flx/button.h"

class MenuState : public flx::State
{
private:
    flx::EmitterPtr _gibs;
    flx::ButtonPtr _b;
    flx::TextPtr _t1;
    flx::TextPtr _t2;
    bool _ok;
    bool _ok2;

public:
    virtual void create();
    virtual void update();
};

