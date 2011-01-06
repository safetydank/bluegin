#include "bluegin/resourcemanager.h"

#include "flx/flx.h"
#include "flx/flxG.h"

#include "rapidxml.hpp"

#include "mode/Mode.h"
#include "mode/PlayStateTiles.h"
#include "mode/Player.h"
#include "mode/Bullet.h"
#include "mode/BotBullet.h"
#include "mode/Spawner.h"

#include "cinder/Color.h"

using namespace ci;
using namespace bluegin;
using namespace flx;
using namespace rapidxml;

extern FlxGlobal FlxG;

void PlayStateTiles::create()
{
    ResourceManager& res = *(FlxG.resources);

    //get the gibs set up and out of the way
    _littleGibs = EmitterPtr(new Emitter());
    _littleGibs->delay = 3;
    _littleGibs->setXSpeed(-150,150);
    _littleGibs->setYSpeed(-200,0);
    _littleGibs->setRotation(-720,-720);
    _littleGibs->createSprites(res.graphic("gibs"),100,10,true,0.5f/*,0.65f*/);
    _bigGibs = EmitterPtr(new Emitter());
    _bigGibs->setXSpeed(-200,200);
    _bigGibs->setYSpeed(-300,0);
    _bigGibs->setRotation(-720,-720);
    _bigGibs->createSprites(res.graphic("spawner_gibs"),50,20,true,0.5/*,0.35*/);
    
    _bullets = GroupPtr(new Group());
    _player = shared_ptr<Player>(new Player(0,0,_bullets->members,_littleGibs));
    _bots = GroupPtr(new Group());
    _botBullets = GroupPtr(new Group());
    _spawners = GroupPtr(new Group());

    //create tilemap
    _tilemap = TilemapPtr(new Tilemap());
    _tilemap->collideIndex = 1;
    shared_ptr<OgmoLoader> loader(new OgmoLoader("mode_level.oel"));
    loader->loadTiles(*_tilemap, "tilesAbove");

    //create spawners
    vector<OgmoObject> spawnerObjs;
    loader->loadObjects(spawnerObjs, "actors", "spawner");
    for (vector<OgmoObject>::iterator it = spawnerObjs.begin(); it != spawnerObjs.end(); ++it) {
        shared_ptr<Spawner> spawner(new Spawner(it->x, it->y, _bigGibs, _bots, _botBullets->members, _littleGibs, _player));
        _spawners->add(spawner);
    }

    add(_spawners);
    add(_littleGibs);
    add(_bigGibs);
    add(_bots);

    for(int i = 0; i < 50; i++)
        _botBullets->add(ObjectPtr(new BotBullet()));
    for(int i = 0; i < 8; i++)
        _bullets->add(ObjectPtr(new Bullet()));

    //add player and set up camera
    OgmoObject mapObject;
    loader->loadObject(mapObject, "actors", "player");
    _player->x = mapObject.x; _player->y = mapObject.y;
    FlxG.follow(_player,2.5);
    FlxG.followAdjust(0.5,0.0);

    _tilemap->follow();	//Set the followBounds to the map dimensions

    add(_tilemap);
    add(_player);

    //add gibs + bullets to scene here, so they're drawn on top of pretty much everything
    add(_botBullets);
    add(_bullets);

    _enemies = GroupPtr(new Group());
    _enemies->add(_botBullets);
    _enemies->add(_spawners);
    _enemies->add(_bots);
    _objects = GroupPtr(new Group());
    _objects->add(_botBullets);
    _objects->add(_bullets);
    _objects->add(_bots);
    _objects->add(_player);
    _objects->add(_littleGibs);
    _objects->add(_bigGibs);

    //Uncomment these lines if you want to center TxtMap2
    //var fx:uint = _tilemap.width/2 - FlxG.width/2;
    //var fy:uint = _tilemap.height/2 - FlxG.height/2;
    //FlxG.followBounds(fx,fy,fx,fy);

    //fade in
    FlxG.playMusic(res.sound("mode"));
    FlxG.flash.start(Color(0.075f, 0.11f, 0.10f));

    //The music in this mode is positional - it fades out toward the edges of the level
    // var s:FlxSound = FlxG.play(SndMode,1,true);
    // s.proximity(320,320,_player,160);
}

static bool overlapped(Object& Object1, Object& Object2)
{
    if((Object1.kind() == BOT_BULLET) || (Object1.kind() == BULLET))
        Object1.kill();
    Object2.hurt(1);

    return true;
}
		

void PlayStateTiles::update()
{
    State::update();
    // _tilemap->collide(*_player);
    // _tilemap->collide(*_bullets);

    FlxU::collide(*_tilemap,*_objects);
    FlxU::overlap(*_enemies,*_player,overlapped);
    FlxU::overlap(*_bullets,*_enemies,overlapped);

    if(_reload)
        FlxG.setState(StatePtr(new PlayState()));
}

