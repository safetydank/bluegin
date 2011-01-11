#include "mode/Bot.h"
#include "mode/BotBullet.h"
#include "bluegin/resourcemanager.h"
#include "flx/sprite.h"
#include "flx/flxG.h"

using namespace bluegin;
using namespace flx;
using namespace ci;

extern FlxGlobal FlxG;

int Bot::_cb = 0;

Bot::Bot(int xPos,int yPos,vector<ObjectPtr>& Bullets,EmitterPtr Gibs,shared_ptr<Player> ThePlayer)
: Sprite(xPos, yPos),
  _player(ThePlayer),
  _b(Bullets),
  _gibs(Gibs)
{
    ResourceManager& res = *(FlxG.resources);
    loadGraphic(res.graphic("bot"), false);

    width = 12;
    height = 12;
    offset.x = 2;
    offset.y = 2;
    maxAngular = 120;
    angularDrag = 400;
    maxThrust = 100;
    drag.x = 80;
    drag.y = 80;

    //Jet effect that shoots out from behind the bot
    _jets = Emitter::create();
    _jets->setRotation();
    _jets->gravity = 0;
    _jets->createSprites(res.graphic("jet"),15,0,false);

    reset(x,y);
}

void Bot::update()
{			
    float ot = _timer;
    // if((_timer == 0) && onScreen()) FlxG.play(SndJet);
    _timer += FlxG.elapsed;
    if((ot < 8) && (_timer >= 8))
        _jets->stop(0.1);

    //Aiming
    float dx = x-_player->x;
    float dy = y-_player->y;
    float da = FlxU::getAngle(dx,dy);
    if(da < 0)
        da += 360;
    float ac = angle;
    if(ac < 0)
        ac += 360;
    if(da < angle)
        angularAcceleration = -angularDrag;
    else if(da > angle)
        angularAcceleration = angularDrag;
    else
        angularAcceleration = 0;

    //Jets
    thrust = 0;
    if(_timer > 9)
        _timer = 0;
    else if(_timer < 8)
    {
        thrust = 40;
        Vec2f v = FlxU::rotatePoint(thrust,0,0,0,angle);
        _jets->at(*this);
        _jets->setXSpeed(v.x-30,v.x+30);
        _jets->setYSpeed(v.y-30,v.y+30);
        if(!_jets->on)
            _jets->start(false,0.01,0);
    }

    //Shooting
    if(onScreen())
    {
        float os = _shotClock;
        _shotClock += FlxG.elapsed;
        if((os < 4.0) && (_shotClock >= 4.0))
        {
            _shotClock = 0;
            shoot();
        }
        else if((os < 3.5) && (_shotClock >= 3.5))
            shoot();
        else if((os < 3.0) && (_shotClock >= 3.0))
            shoot();
    }

    _jets->update();
    Sprite::update();
}

void Bot::render()
{
    _jets->render();
    Sprite::render();
}

void Bot::hurt(float Damage)
{
    // FlxG.play(SndHit);
    flicker(0.2);
    FlxG.score += 10;
    Sprite::hurt(Damage);
}

void Bot::kill()
{
    ResourceManager& res = *(FlxG.resources);

    if(dead)
        return;
    FlxG.play(res.sound("SndExplode"));
    Sprite::kill();
    flicker(-1);
    _jets->kill();
    _gibs->at(*this);
    _gibs->start(true,0,20);
    FlxG.score += 200;
}

void Bot::reset(float X, float Y)
{
    Sprite::reset(X,Y);
    thrust = 0;
    velocity.x = 0;
    velocity.y = 0;
    angle = FlxU::random()*360 - 180;
    health = 2;
    _timer = 0;
    _shotClock = 0;
}

void Bot::shoot()
{
    Vec2f ba = FlxU::rotatePoint(-120,0,0,0,angle);
    BotBullet& bullet = static_cast<BotBullet&>(*(_b[_cb]));
    bullet.shoot(x+width/2-2,y+height/2-2,ba.x,ba.y);
    if(++_cb >= _b.size()) _cb = 0;
}
