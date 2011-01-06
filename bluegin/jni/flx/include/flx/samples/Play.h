#pragma once

#include "flx/state.h"
#include "cinder/gl/Texture.h"
#include "bluegin/hgeFont.h"
#include "flx/text.h"

class Play : public flx::State
{
public:
    Play();
    virtual ~Play();

    void create();
    void update();
    void preProcess();
    void postProcess();
};
