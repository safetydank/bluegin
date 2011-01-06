#include "flx/data/fade.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace flx;

Fade::Fade() : alpha(0)
{
    // super();
    // createGraphic(FlxG.width,FlxG.height,0,true);
    //scrollFactor.x = 0;
    //scrollFactor.y = 0;
    exists = false;
}

void Fade::start(Color color, float Duration, FadeCallback FadeComplete, bool Force)
{
    if (!Force && exists) return;
    // fill(color);
    _delay = Duration;
    _color = color;
    _complete = FadeComplete;
    _completeData = NULL;
    alpha = 0;
    exists = true;
}

void Fade::stop()
{
    exists = false;
}

void Fade::update(float elapsed)
{
    alpha += elapsed / _delay;
    if (alpha >= 1)
    {
        alpha = 1;
        if(_complete != NULL)
            _complete(_completeData);
    }
    fadeColor = ColorA(_color.r, _color.g, _color.b, alpha);
}

void Fade::setCallbackData(void* data)
{
    _completeData = data;
}

