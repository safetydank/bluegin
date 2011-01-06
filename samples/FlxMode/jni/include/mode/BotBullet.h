#pragma once

#include "flx/sprite.h"

class BotBullet : public flx::Sprite
{
public:
    BotBullet();

    virtual void update();
    virtual void hitLeft(flx::Object& Contact, float Velocity);
    virtual void hitRight(flx::Object& Contact, float Velocity);
    virtual void hitBottom(flx::Object& Contact, float Velocity);
    virtual void hitTop(flx::Object& Contact, float Velocity);
    virtual void kill();

    void shoot(int X, int Y, int VelocityX, int VelocityY);
};
