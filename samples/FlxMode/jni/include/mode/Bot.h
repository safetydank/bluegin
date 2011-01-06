#pragma once

#include "flx/sprite.h"
#include "flx/emitter.h"
#include "flx/object.h"
#include "mode/Player.h"

class Bot : public flx::Sprite
{
protected:
    flx::EmitterPtr _gibs;
    flx::EmitterPtr _jets;
    shared_ptr<Player> _player;
    float _timer;
    vector<flx::ObjectPtr> _b;
    static int _cb;
    float _shotClock;

public:
    Bot(int xPos,int yPos,vector<flx::ObjectPtr>& Bullets,flx::EmitterPtr Gibs,shared_ptr<Player> ThePlayer);
    
    virtual void update();
    virtual void render();
    virtual void hurt(float Damage);
    virtual void kill();
    virtual void reset(float X, float Y);

protected:
    void shoot();
};

