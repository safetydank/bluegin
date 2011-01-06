#pragma once

#include "flx/state.h"
#include "flx/group.h"
#include "flx/emitter.h"
#include "flx/text.h"
#include "mode/Player.h"

class ReloadableState : public flx::State
{
public:
    ReloadableState() : flx::State(), _reload(false) { }
    void reload() { _reload = true; }
protected:
    bool _reload;
};

//  Mode port
class PlayState : public ReloadableState
{
    // [Embed(source="../../../data/tech_tiles.png")] protected var ImgTech:Class;
    // [Embed(source="../../../data/dirt_top.png")] protected var ImgDirtTop:Class;
    // [Embed(source="../../../data/dirt.png")] protected var ImgDirt:Class;
    // [Embed(source="../../../data/notch.png")] protected var ImgNotch:Class;
    // [Embed(source="../../../data/mode.mp3")] protected var SndMode:Class;
    // [Embed(source="../../../data/countdown.mp3")] protected var SndCount:Class;
    // [Embed(source="../../../data/gibs.png")] private var ImgGibs:Class;
    // [Embed(source="../../../data/spawner_gibs.png")] private var ImgSpawnerGibs:Class;

protected:
    //major game objects
    flx::GroupPtr _blocks;
    flx::GroupPtr _decorations;
    flx::GroupPtr _bullets;
    shared_ptr<Player> _player;
    flx::GroupPtr _bots;
    flx::GroupPtr _spawners;
    flx::GroupPtr _botBullets;
    flx::EmitterPtr _littleGibs;
    flx::EmitterPtr _bigGibs;

    //meta groups, to help speed up collisions
    flx::GroupPtr _objects;
    flx::GroupPtr _enemies;

    //HUD
    flx::TextPtr _score;
    flx::TextPtr _score2;
    float _scoreTimer;
    float _jamTimer;
    flx::SpritePtr _jamBar;
    flx::TextPtr _jamText;
    vector<flx::SpritePtr> _notches;

    // XXX debugging
    flx::TextPtr _accelText;

    //just to prevent weirdness during level transition
    bool _fading;

    float _fpsDelay;
    int _renderedFrames;

public:
    virtual void create();
    virtual void update();
    
    // protected function overlapped(Object1:FlxObject,Object2:FlxObject):void
    // {
    //     if((Object1 is BotBullet) || (Object1 is Bullet))
    //         Object1.kill();
    //     Object2.hurt(1);
    // }

    // protected function onVictory():void
    // {
    //     FlxG.music.stop();
    //     FlxG.state = new VictoryState();
    // }

    //Just plops down a spawner and some blocks - haphazard and crappy atm but functional!
    void buildRoom(int RX, int RY, bool Spawners=false);
};
