#include "flx/flxG.h"
#include "mode/Mode.h"
#include "mode/Bullet.h"

using namespace bluegin;
using namespace ci;
using namespace flx;

extern FlxGlobal FlxG;

Bullet::Bullet() : Sprite(0, 0)
{
    ResourceManager& res = *(FlxG.resources);

    loadGraphic(res.graphic("bullet"),true);
    width = 6;
    height = 6;
    offset.x = 1;
    offset.y = 1;
    exists = false;

    int upFrames[] = { 0 };
    addAnimation("up", 1, upFrames);
    int downFrames[] = { 1 };
    addAnimation("down", 1, downFrames);
    int leftFrames[] = { 2 };
    addAnimation("left", 1, leftFrames);
    int rightFrames[] = { 3 };
    addAnimation("right", 1, rightFrames);
    int poofFrames[] = { 4, 5, 6, 7 };
    addAnimation("poof", 4, poofFrames, 50, false);

    kind(BULLET);
}

void Bullet::update()
{
    if(dead && finished) 
        exists = false;
    else 
        Sprite::update();
}

void Bullet::render()
{
    Sprite::render();
}

void Bullet::hitLeft(Object& Contact, float Velocity) 
{ 
    kill(); 
}

void Bullet::hitRight(Object& Contact, float Velocity) 
{ 
    kill(); 
}

void Bullet::hitBottom(Object& Contact, float Velocity) 
{ 
    kill(); 
}

void Bullet::hitTop(Object& Contact, float Velocity) 
{ 
    kill(); 
}

void Bullet::kill()
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

void Bullet::shoot(int X, int Y, int VelocityX, int VelocityY)
{
    ResourceManager& res = *(FlxG.resources);

    FlxG.play(res.sound("SndShoot"), 0.2f);
    Sprite::reset(X,Y);
    solid = true;
    velocity.x = VelocityX;
    velocity.y = VelocityY;
    if(velocity.y < 0)
        play("up");
    else if(velocity.y > 0)
        play("down");
    else if(velocity.x < 0)
        play("left");
    else if(velocity.x > 0)
        play("right");
}


