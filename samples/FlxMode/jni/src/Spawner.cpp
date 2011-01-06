#include "bluegin/resourcemanager.h"
#include "flx/flxG.h"
#include "mode/Spawner.h"
#include "mode/Bot.h"

using namespace bluegin;
using namespace flx;
using namespace ci;

extern FlxGlobal FlxG;

Spawner::Spawner(int X, int Y, EmitterPtr Gibs, GroupPtr Bots, vector<ObjectPtr>& BotBullets,
        EmitterPtr BotGibs, shared_ptr<Player> ThePlayer)
: Sprite(X, Y), 
    _gibs(Gibs), 
    _bots(Bots), 
    _botBullets(BotBullets), 
    _botGibs(BotGibs), 
    _player(ThePlayer)
{
    ResourceManager& res = *(FlxG.resources);
    loadGraphic(res.graphic("spawner"),true);
    _timer = FlxU::random()*20;
    _open = false;
    health = 8;

    int openFrames[] = { 1, 2, 3, 4, 5 };
    addAnimation("open", 5, openFrames, 40, false);
    int closeFrames[] = { 4, 3, 2, 1, 0 };
    addAnimation("close", 5, closeFrames, 40, false);
    int deadFrames[] = { 6 };
    addAnimation("dead", 1, deadFrames);
}

void Spawner::update()
{
    _timer += FlxG.elapsed;
    int limit = 20;
    if(onScreen())
        limit = 4;
    if(_timer > limit)
    {
        _timer = 0;
        makeBot();
    }
    else if(_timer > limit - 0.35)
    {
        if(!_open)
        {
            _open = true;
            play("open");
        }
    }
    else if(_timer > 1)
    {
        if(_open)
        {
            play("close");
            _open = false;
        }
    }

    Sprite::update();
}

void Spawner::hurt(float Damage)
{
    ResourceManager& res = *(FlxG.resources);

    FlxG.play(res.sound("SndHit"));
    // flicker(0.2);
    FlxG.score += 50;
    Sprite::hurt(Damage);
}

void Spawner::kill()
{
    ResourceManager& res = *(FlxG.resources);

    if(dead)
        return;
    FlxG.play(res.sound("SndExplode"));
    FlxG.play(res.sound("SndExplode2"));
    Sprite::kill();
    active = false;
    exists = true;
    solid = false;
    // flicker(-1);
    play("dead");
    FlxG.quake.start(0.005,0.35);
    FlxG.flash.start(Color(0.85f, 0.92f, 0.64f),0.35);
    makeBot();
    _gibs->at(*this);
    _gibs->start(true,3,0);
    FlxG.score += 1000;
}

void Spawner::makeBot()
{
    //Try to recycle a dead bot
    if(_bots->resetFirstAvail(x + width/2 - 6, y + height/2 - 6))
        return;

    //If there weren't any non-existent ones to respawn, just add a new one instead
    shared_ptr<Bot> bot(new Bot(x + width/2, y + height/2, _botBullets, _botGibs, _player));
    bot->x -= bot->width/2;
    bot->y -= bot->height/2;
    _bots->add(bot);
}

