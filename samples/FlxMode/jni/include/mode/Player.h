#pragma once

#include "flx/sprite.h"
#include "flx/emitter.h"

class Player : public flx::Sprite
{
private:
    int _jumpPower;
    vector<flx::ObjectPtr>& _bullets;
    int _curBullet;
    int _bulletVel;
    float _bulletDelay;
    bool _up;
    bool _down;
    float _restart;
    flx::EmitterPtr _gibs;
    flx::EmitterPtr _jets;

public:
    Player(int X, int Y, vector<flx::ObjectPtr>& Bullets, flx::EmitterPtr Gibs);

    virtual void update();
    virtual void render();
    virtual void hitLeft(flx::Object& Contact, float Velocity);
    virtual void hitRight(flx::Object& Contact, float Velocity);
    virtual void hitTop(flx::Object& Contact, float Velocity);
    virtual void hitBottom(flx::Object& Contact, float Velocity);
    virtual void hurt(float Damage);
    virtual void kill();
};
