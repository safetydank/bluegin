#pragma once

#include "flx/sprite.h"
#include "flx/group.h"
#include "flx/object.h"
#include "flx/emitter.h"
#include "mode/Player.h"

class Spawner : public flx::Sprite
{
private:
    float _timer;
    flx::GroupPtr _bots;
    vector<flx::ObjectPtr>& _botBullets;
    flx::EmitterPtr _botGibs;
    flx::EmitterPtr _gibs;
    shared_ptr<Player> _player;
    bool _open;

public:
    // XXX replace shared pointers with references?
    Spawner(int X, int Y, flx::EmitterPtr Gibs, flx::GroupPtr Bots, vector<flx::ObjectPtr>& BotBullets,
            flx::EmitterPtr BotGibs, shared_ptr<Player> ThePlayer);

    void update();
    void hurt(float Damage);
    void kill();
    void makeBot();
};

