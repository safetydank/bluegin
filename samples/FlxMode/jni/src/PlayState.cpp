#include "bluegin/resourcemanager.h"

#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/tileblock.h"
#include "mode/Mode.h"
#include "mode/MenuState.h"
#include "mode/PlayState.h"
#include "mode/PlayStateTiles.h"
#include "mode/Player.h"
#include "mode/Bullet.h"
#include "mode/BotBullet.h"
#include "mode/Spawner.h"

#include "cinder/Color.h"

using namespace ci;
using namespace bluegin;
using namespace flx;

extern FlxGlobal FlxG;

void PlayState::create()
{
    ResourceManager& res = *(FlxG.resources);

    bgColor = Color(1.0f, 1.0f, 0);
    // FlxG.mouse.hide();
    _reload = false;

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

    //level generation needs to know about the spawners (and thusly the bots, players, etc)
    _blocks = GroupPtr(new Group());
    _decorations = GroupPtr(new Group());
    _bullets = GroupPtr(new Group());
    _player = shared_ptr<Player>(new Player(300,311,_bullets->members,_littleGibs));
    _bots = GroupPtr(new Group());
    _botBullets = GroupPtr(new Group());
    _spawners = GroupPtr(new Group());

    //simple procedural level generation
    int i;
    int r = 160;
    TileblockPtr b;

    b = TileblockPtr(new Tileblock(0,0,640,16));
    b->loadGraphic(res.graphic("tech_tiles"));
    _blocks->add(b);

    b = TileblockPtr(new Tileblock(0,16,16,640-16));
    b->loadGraphic(res.graphic("tech_tiles"));
    _blocks->add(b);

    b = TileblockPtr(new Tileblock(640-16,16,16,640-16));
    b->loadGraphic(res.graphic("tech_tiles"));
    _blocks->add(b);

    b = TileblockPtr(new Tileblock(16,640-24,640-32,8));
    b->loadGraphic(res.graphic("dirt_top"));
    _blocks->add(b);

    b = TileblockPtr(new Tileblock(16,640-16,640-32,16));
    b->loadGraphic(res.graphic("dirt"));
    _blocks->add(b);

    buildRoom(r*0,r*0,true);
    buildRoom(r*1,r*0);
    buildRoom(r*2,r*0);
    buildRoom(r*3,r*0,true);
    buildRoom(r*0,r*1,true);
    buildRoom(r*1,r*1);
    buildRoom(r*2,r*1);
    buildRoom(r*3,r*1,true);
    buildRoom(r*0,r*2);
    buildRoom(r*1,r*2);
    buildRoom(r*2,r*2);
    buildRoom(r*3,r*2);
    buildRoom(r*0,r*3,true);
    buildRoom(r*1,r*3);
    buildRoom(r*2,r*3);
    buildRoom(r*3,r*3,true);

    //Add bots and spawners after we add blocks to the state,
    // so that they're drawn on top of the level, and so that
    // the bots are drawn on top of both the blocks + the spawners.
    add(_spawners);
    add(_littleGibs);
    add(_bigGibs);
    add(_blocks);
    add(_decorations);
    add(_bots);

    //actually create the bullets now
    for(i = 0; i < 50; i++)
        _botBullets->add(ObjectPtr(new BotBullet()));
    for(i = 0; i < 8; i++)
        _bullets->add(ObjectPtr(new Bullet()));

    //add player and set up scrolling camera
    add(_player);
    FlxG.follow(_player, 2.5);
    FlxG.followAdjust(0.5f,0.0);
    FlxG.followBounds(0,0,640,640);

    //add gibs + bullets to scene here, so they're drawn on top of pretty much everything
    add(_botBullets);
    add(_bullets);

    //finally we are going to sort things into a couple of helper groups.
    //we don't add these to the state, we just use them for collisions later!
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

    //HUD - score
    // var ssf:FlxPoint = new FlxPoint(0,0);
    // _score = new FlxText(0,0,FlxG.width);
    // _score.color = 0xd8eba2;
    // _score.size = 16;
    // _score.alignment = "center";
    // _score.scrollFactor = ssf;
    // _score.shadow = 0x131c1b;
    // add(_score);
    // if(FlxG.scores.length < 2)
    // {
    //     FlxG.scores.push(0);
    //     FlxG.scores.push(0);
    // }

    _score = TextPtr(new Text(0, 0, 100));
    _score->setFont(res.font("nokia"));
    _score->setColor(ColorA(0.85f, 0.92f, 0.64f, 0.9f));
    _score->setSize(18.0f);
    _score->printf("%d", FlxG.score);
    add(_score);

    // //HUD - highest and last scores
    // _score2 = new FlxText(FlxG.width/2,0,FlxG.width/2)
    //     _score2.color = 0xd8eba2;
    // _score2.alignment = "right";
    // _score2.scrollFactor = ssf;
    // _score2.shadow = _score.shadow;
    // add(_score2);
    // if(FlxG.score > FlxG.scores[0])
    //     FlxG.scores[0] = FlxG.score;
    // if(FlxG.scores[0] != 0)
    //     _score2.text = "HIGHEST: "+FlxG.scores[0]+"\nLAST: "+FlxG.score;
    // FlxG.score = 0;
    // _scoreTimer = 0;

    // //HUD - the "number of spawns left" icons
    // _notches = new Array();
    // var tmp:FlxSprite;
    // for(i = 0; i < 6; i++)
    // {
    //     tmp = new FlxSprite(4+i*10,4);
    //     tmp.loadGraphic(ImgNotch,true);
    //     tmp.scrollFactor.x = tmp.scrollFactor.y = 0;
    //     tmp.addAnimation("on",[0]);
    //     tmp.addAnimation("off",[1]);
    //     tmp.moves = false;
    //     tmp.solid = false;
    //     tmp.play("on");
    //     _notches.push(this.add(tmp));
    // }

    // //HUD - the "gun jammed" notification
    // _jamBar = this.add((new FlxSprite(0,FlxG.height-22)).createGraphic(FlxG.width,24,0xff131c1b)) as FlxSprite;
    // _jamBar.scrollFactor.x = _jamBar.scrollFactor.y = 0;
    // _jamBar.visible = false;
    // _jamText = new FlxText(0,FlxG.height-22,FlxG.width,"GUN IS JAMMED");
    // _jamText.color = 0xd8eba2;
    // _jamText.size = 16;
    // _jamText.alignment = "center";
    // _jamText.scrollFactor = ssf;
    // _jamText.visible = false;
    // add(_jamText);

    FlxG.playMusic(res.sound("mode"));
    FlxG.flash.start(Color(0.075f, 0.11f, 0.10f));
    _fading = false;
    State::bgColor = Color::black();

     _accelText = TextPtr(new Text(0, 0, 100, "HELLO"));
     _accelText->setFont(res.font("nokia"));
     _accelText->setSize(12.0f);
    // add(_accelText);

    _fpsDelay = 0;
    _renderedFrames = 0;
}

static bool overlapped(Object& Object1, Object& Object2)
{
    if((Object1.kind() == BOT_BULLET) || (Object1.kind() == BULLET))
        Object1.kill();
    Object2.hurt(1);

    return true;
}
		

void PlayState::update()
{
    int os = FlxG.score;

    State::update();

    //collisions with environment
    FlxU::collide(*_blocks,*_objects);
    FlxU::overlap(*_enemies,*_player,overlapped);
    FlxU::overlap(*_bullets,*_enemies,overlapped);

    _renderedFrames++;
    if (_fpsDelay > 0)
        _fpsDelay -= FlxG.elapsed;
    if (_fpsDelay <= 0) {
        _accelText->printf("%.1f FPS", (_renderedFrames / (1.0f - _fpsDelay)));
        _renderedFrames = 0;
        _fpsDelay = 1.0f;
    }

    // //Jammed message
    // if(FlxG.keys.justPressed("C") && _player.flickering())
    // {
    //     _jamTimer = 1;
    //     _jamBar.visible = true;
    //     _jamText.visible = true;
    // }
    // if(_jamTimer > 0)
    // {
    //     if(!_player.flickering()) _jamTimer = 0;
    //     _jamTimer -= FlxG.elapsed;
    //     if(_jamTimer < 0)
    //     {
    //         _jamBar.visible = false;
    //         _jamText.visible = false;
    //     }
    // }

    // if(!_fading)
    // {
    //     //Score + countdown stuffs
    //     if(os != FlxG.score) _scoreTimer = 2;
    //     _scoreTimer -= FlxG.elapsed;
    //     if(_scoreTimer < 0)
    //     {
    //         if(FlxG.score > 0) 
    //         {
    //             FlxG.play(SndCount);
    //             if(FlxG.score > 100) FlxG.score -= 100;
    //             else { FlxG.score = 0; _player.kill(); }
    //             _scoreTimer = 1;
    //             if(FlxG.score < 600)
    //                 FlxG.play(SndCount);
    //             if(FlxG.score < 500)
    //                 FlxG.play(SndCount);
    //             if(FlxG.score < 400)
    //                 FlxG.play(SndCount);
    //             if(FlxG.score < 300)
    //                 FlxG.play(SndCount);
    //             if(FlxG.score < 200)
    //                 FlxG.play(SndCount);
    //         }
    //     }

    //     //Fade out to victory screen stuffs
    //     var spawnerCount:int = _spawners.countLiving();
    //     if(spawnerCount <= 0)
    //     {
    //         _fading = true;
    //         FlxG.fade.start(0xffd8eba2,3,onVictory);
    //     }
    //     else
    //     {
    //         var l:uint = _notches.length;
    //         for(var i:uint = 0; i < l; i++)
    //         {
    //             if(i < spawnerCount)
    //                 _notches[i].play("on");
    //             else
    //                 _notches[i].play("off");
    //         }
    //     }
    // }

    //actually update score text if it changed
    if(os != FlxG.score)
    {
        if(_player->dead) FlxG.score = 0;
        _score->printf("%d", FlxG.score);
    }

    if(_reload) {
        FlxG.music->stop();
        FlxG.setState(StatePtr(new MenuState()));
    }
}

void PlayState::buildRoom(int RX, int RY, bool Spawners)
{
    ResourceManager& res = *(FlxG.resources);

    //first place the spawn point (if necessary)
    int rw = 20;
    int sx;
    int sy;
    if(Spawners)
    {
        sx = 2+FlxU::random()*(rw-7);
        sy = 2+FlxU::random()*(rw-7);
    }

    //then place a bunch of blocks
    int numBlocks = 3+FlxU::random()*4;
    if(!Spawners) numBlocks++;
    int maxW = 10;
    int minW = 2;
    int maxH = 8;
    int minH = 1;
    int bx;
    int by;
    int bw;
    int bh;
    bool check;
    for(int i = 0; i < numBlocks; i++)
    {
        check = false;
        do
        {
            //keep generating different specs if they overlap the spawner
            bw = minW + FlxU::random()*(maxW-minW);
            bh = minH + FlxU::random()*(maxH-minH);
            bx = -1 + FlxU::random()*(rw+1-bw);
            by = -1 + FlxU::random()*(rw+1-bh);
            if(Spawners)
                check = ((sx>bx+bw) || (sx+3<bx) || (sy>by+bh) || (sy+3<by));
            else
                check = true;
        } while(!check);

        TileblockPtr b;

        b = TileblockPtr(new Tileblock(RX+bx*8,RY+by*8,bw*8,bh*8));
        b->loadGraphic(res.graphic("tech_tiles"));
        _blocks->add(b);

        //If the block has room, add some non-colliding "dirt" graphics for variety
        if((bw >= 4) && (bh >= 5))
        {
            b = TileblockPtr(new Tileblock(RX+bx*8+8,RY+by*8,bw*8-16,8));
            b->loadGraphic(res.graphic("dirt_top"));
            _decorations->add(b);

            b = TileblockPtr(new Tileblock(RX+bx*8+8,RY+by*8+8,bw*8-16,bh*8-24));
            b->loadGraphic(res.graphic("dirt"));
            _decorations->add(b);
        }
    }

    //Finally actually add the spawner
    if(Spawners) {
        shared_ptr<Spawner> spawner(new Spawner(RX+sx*8,RY+sy*8,_bigGibs,_bots,_botBullets->members,_littleGibs,_player));
        _spawners->add(spawner);
    }
}

