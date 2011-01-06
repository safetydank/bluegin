#include "flx/tilemap.h"
#include "flx/group.h"
#include "flx/state.h"
#include "flx/emitter.h"

#include "mode/PlayState.h"
#include "mode/Player.h"

class PlayStateTiles : public ReloadableState
{
    //major game objects
    flx::TilemapPtr _tilemap;
    flx::GroupPtr _bullets;
    flx::GroupPtr _spawners;
    flx::GroupPtr _bots;
    flx::GroupPtr _botBullets;
    shared_ptr<Player> _player;

    flx::EmitterPtr _littleGibs;
    flx::EmitterPtr _bigGibs;

    //meta groups, to help speed up collisions
    flx::GroupPtr _objects;
    flx::GroupPtr _enemies;

    virtual void create();
    virtual void update();
};

