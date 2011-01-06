#include "bluegin/resourcemanager.h"
#include "flx/flxG.h"
#include "mode/Mode.h"
#include "mode/BotBullet.h"

using namespace ci;
using namespace flx;
using namespace bluegin;

extern FlxGlobal FlxG;

BotBullet::BotBullet() : Sprite(0,0)
{
    ResourceManager& res = *(FlxG.resources);

    loadGraphic(res.graphic("bot_bullet"),true);
    int idleFrames[] = { 0, 1 };
    addAnimation("idle", 2, idleFrames, 50);
    int poofFrames[] = { 2, 3, 4 };
    addAnimation("poof", 3, poofFrames, 50, false);
    exists = false;

    kind(BOT_BULLET);
}

void BotBullet::update()
{
    if(dead && finished) exists = false;
    else Sprite::update();
}

void BotBullet::hitLeft(flx::Object& Contact, float Velocity) { kill(); }
void BotBullet::hitRight(flx::Object& Contact, float Velocity) { kill(); }
void BotBullet::hitBottom(flx::Object& Contact, float Velocity) { kill(); }
void BotBullet::hitTop(flx::Object& Contact, float Velocity) { kill(); }

void BotBullet::kill()
{
    ResourceManager& res = *(FlxG.resources);

    if(dead) return;
    velocity.x = 0;
    velocity.y = 0;
    if(onScreen()) FlxG.play(res.sound("SndHit"), 0.2f);
    dead = true;
    solid = false;
    play("poof");
}

void BotBullet::shoot(int X, int Y, int VelocityX, int VelocityY)
{
    ResourceManager& res = *(FlxG.resources);

    FlxG.play(res.sound("SndShoot"),0.1f);
    Sprite::reset(X,Y);
    solid = true;
    velocity.x = VelocityX;
    velocity.y = VelocityY;
    play("idle");
}

