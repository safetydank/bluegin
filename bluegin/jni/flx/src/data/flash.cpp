#include "flx/data/flash.h"

using namespace flx;
using namespace ci;

Flash::Flash()
{
    exists = false;
}

void Flash::start(Color color, float Duration, void* FlashComplete, bool Force) 
{
    this->color = color;
    if (!Force && exists) return;
    _delay = Duration;
    // _complete = FlashComplete;
    alpha = 1;
    exists = true;
}

void Flash::stop()
{
    exists = false;
}

void Flash::update(float elapsed)
{
    alpha -= elapsed/_delay;
    if(alpha <= 0)
    {
        exists = false;
        // if(_complete != null)
        // 	_complete();
    }
}

