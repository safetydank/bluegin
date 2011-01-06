#include "flx/data/anim.h"

using namespace flx;
using std::vector;

Anim::Anim(std::string name, vector<int>& frames, float frameRate, bool looped)
    : name(name), frames(frames), looped(looped)
{
    delay = 0;
    if (frameRate > 0)
        delay = 1.0f / frameRate;
}

