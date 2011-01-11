#include "flx/flxG.h"
#include "flx/emitter.h"
#include "flx/sprite.h"

using namespace flx;
using namespace cinder;

extern FlxGlobal FlxG;

EmitterPtr Emitter::create(float X, float Y)
{
    return EmitterPtr(new Emitter(X, Y));
}
    
Emitter::Emitter(float X, float Y) : Group()
{
    x = X;
    y = Y;

    width = 0;
    height = 0;
    
    minParticleSpeed.set(-100,-100);
    maxParticleSpeed.set(100,100);
    minRotation = -360.0f;
    maxRotation = 360.0f;
    gravity = 400.0f;
    particleDrag.set(0, 0);
    delay = 0;
    quantity = 0;
    _counter = 0;
    _explode = true;
    exists = false;
    on = false;
}

Emitter::~Emitter()
{
}

Emitter& Emitter::createSprites(bluegin::Graphic Graphics, 
        int Quantity, int BakedRotations, bool Multiple, float Collide)
{
    members.clear();
    int r;
    SpritePtr s;
    int tf = 1;
    float sw;
    float sh;
    if(Multiple)
    {
        s  = Sprite::create(0, 0, Graphics);
        tf = static_cast<int>(s->width / s->height);
    }
    for(int i = 0; i < Quantity; ++i)
    {
        s = Sprite::create();
        if(Multiple)
        {
            r = static_cast<int>(FlxU::random()*tf);
            s->loadGraphic(Graphics,true);
            s->setFrame(r);
        }
        else
        {
            s->loadGraphic(Graphics);
        }
        if (Collide > 0)
        {
            sw = s->width;
            sh = s->height;
            s->width  *= Collide;
            s->height *= Collide;
            s->offset.x = (sw - s->width)/2;
            s->offset.y = (sh - s->height)/2;
            s->solid = true;
        }
        else
            s->solid = false;
        s->exists = false;
        s->scrollFactor = scrollFactor;
        add(s);
    }
    return *this;
}

void Emitter::setSize(float Width, float Height)
{
    width = Width;
    height = Height;
}

void Emitter::setXSpeed(float Min, float Max)
{
    minParticleSpeed.x = Min;
    maxParticleSpeed.x = Max;
}

void Emitter::setYSpeed(float Min, float Max)
{
    minParticleSpeed.y = Min;
    maxParticleSpeed.y = Max;
}

void Emitter::setRotation(float Min, float Max)
{
    minRotation = Min;
    maxRotation = Max;
}

void Emitter::updateEmitter()
{
    if(_explode)
    {
        int i;
        int l;
        _timer += FlxG.elapsed;
        if((delay > 0) && (_timer > delay))
        {
            kill();
            return;
        }
        if(on)
        {
            on = false;
            l = members.size();
            if(quantity > 0)
                l = quantity;
            l += _particle;
            for(i = _particle; i < l; i++)
                emitParticle();
        }
        return;
    }
    if(!on)
        return;
    _timer += FlxG.elapsed;
    while((_timer > delay) && ((quantity <= 0) || (_counter < quantity)))
    {
        _timer -= delay;
        emitParticle();
    }
}


void Emitter::updateMembers()
{
    for (vector<ObjectPtr>::iterator it = members.begin(); it != members.end(); ++it) {
        if (it->get() == NULL)
            continue;

        Object& o = **it;
        if(o.exists && o.active)
            o.update();
    }
}

void Emitter::update()
{
    Group::update();
    updateEmitter();
}

void Emitter::start(bool Explode, float Delay, int Quantity)
{
    if(members.size() <= 0)
    {
        Log("WARNING: there are no sprites loaded in your emitter.\nAdd some to Emitter.members or use Emitter.createSprites().");
        return;
    }
    _explode = Explode;
    if(!_explode)
        _counter = 0;
    if(!exists)
        _particle = 0;
    exists = true;
    visible = true;
    active = true;
    dead = false;
    on = true;
    _timer = 0;
    if(quantity == 0)
        quantity = Quantity;
    if(Delay != 0)
        delay = Delay;
    if(delay < 0)
        delay = -delay;
    if(delay == 0)
    {
        if(Explode)
            delay = 3;      //default value for particle explosions
        else
            delay = 0.2f;   //default value for particle streams
    }
}

void Emitter::emitParticle()
{
    _counter++;
    ObjectPtr sp = members[_particle];
    Sprite& s = static_cast<Sprite&>(*sp);
    s.visible = true;
    s.exists = true;
    s.active = true;
    s.x = x - (s.width / 2.0f) + FlxU::random()*width;
    s.y = y - (s.height / 2.0f) + FlxU::random()*height;
    s.velocity.x = minParticleSpeed.x;
    if(minParticleSpeed.x != maxParticleSpeed.x) s.velocity.x += FlxU::random()*(maxParticleSpeed.x-minParticleSpeed.x);
    s.velocity.y = minParticleSpeed.y;
    if(minParticleSpeed.y != maxParticleSpeed.y) s.velocity.y += FlxU::random()*(maxParticleSpeed.y-minParticleSpeed.y);
    s.acceleration.y = gravity;
    s.angularVelocity = minRotation;
    if(minRotation != maxRotation) s.angularVelocity += FlxU::random()*(maxRotation-minRotation);
    if(s.angularVelocity != 0) s.angle = static_cast<int>(FlxU::random()*360-180);
    s.drag.x = particleDrag.x;
    s.drag.y = particleDrag.y;
    s.visible = true;
    _particle++;
    if(_particle >= members.size())
        _particle = 0;
    s.onEmit();
}

void Emitter::stop(float Delay)
{
    _explode = true;
    delay = Delay;
    if(delay < 0)
        delay = -Delay;
    on = false;
}

void Emitter::at(Object& object)
{
    x = object.x + object.origin.x;
    y = object.y + object.origin.y;
}

void Emitter::kill()
{
    Group::kill();
    on = false;
}


