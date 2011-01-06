#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/data/quake.h"

using namespace flx;

extern FlxGlobal FlxG;

Quake::Quake()
{
    start(0);
}

void Quake::start(float Intensity, float Duration)
{
    stop();
    _intensity = Intensity;
    _timer = Duration;
}

void Quake::stop()
{
    x = 0;
    y = 0;
    _intensity = 0;
    _timer = 0;
}

void Quake::update()
{
    if(_timer > 0)
    {
        _timer -= float(FlxG.elapsed);
        if(_timer <= 0)
        {
            _timer = 0;
            x = 0;
            y = 0;
        }
        else
        {
            x = (FlxU::random()*_intensity*FlxG.width*2-_intensity*FlxG.width);
            y = (FlxU::random()*_intensity*FlxG.height*2-_intensity*FlxG.height);
        }
    }
}

