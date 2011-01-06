#include "bluegin/resourcemanager.h"
#include "mode/MenuState.h"
#include "mode/PlayState.h"
#include "mode/PlayStateTiles.h"
#include "flx/flxG.h"

using namespace ci;
using namespace bluegin;
using namespace flx;

extern FlxGlobal FlxG;

void onFade(void*)
{
    FlxG.setState(StatePtr(new PlayStateTiles()));
    // FlxG.setState(StatePtr(new PlayState()));
}

static void onFlixel(void* data)
{
    Log("XXX onFlixel callback");
}

// private function onDanny():void
// {
//     FlxU.openURL("http://dbsoundworks.com");
// }

static void onButton(void* data)
{
    Log("XXX onButton");
    FlxG.flash.start(Color(0.85f, 0.92f, 0.64f),0.5);
    FlxG.fade.start(Color::black(),1,onFade);
    //_b.visible = false;
    //_b.active = false;
    // FlxG.play(SndHit2);
}

void MenuState::create()
{
    ResourceManager& res = *(FlxG.resources);

    int i;

    _gibs = EmitterPtr(new Emitter(FlxG.width/2-50,FlxG.height/2-10));
    _gibs->setSize(100,30);
    _gibs->setYSpeed(-200,-20);
    _gibs->setRotation(-720,720);
    _gibs->gravity = 100;
    _gibs->createSprites(res.graphic("gibs"),1000,32);
    add(_gibs);

    _t1 = TextPtr(new Text(FlxG.width,FlxG.height/4,80,"mo"));
    _t1->setSize(32.0f);
    _t1->setColor(FlxU::color(0x3a5c39));
    add(_t1);

    _t2 = TextPtr(new Text(-60,FlxG.height/4,80,"de"));
    _t2->setSize(_t1->getSize());
    _t2->setColor(_t1->getColor());
    add(_t2);

    _ok = false;
    _ok2 = false;

    FlxG.score = 0;

    // FlxG.mouse.show(ImgCursor);

    //Simple use of flixel save game object
    // var save:FlxSave = new FlxSave();
    // if(save.bind("Mode"))
    // {
    //     if(save.data.plays == null)
    //         save.data.plays = 0;
    //     else
    //         save.data.plays++;
    //     FlxG.log("Number of plays: "+save.data.plays);
    // }
}

void MenuState::update()
{
    ////Slides the text onto the screen
    int t1m = FlxG.width/2 - 42;
    if(_t1->x > t1m)
    {
        Vec2f pos = _t1->getPos();
        pos.x -= FlxG.elapsed*FlxG.width;
        if (pos.x < t1m) pos.x = t1m;
        _t1->setPos(pos);
    }
    int t2m = FlxG.width/2 + 5;
    if(_t2->x < t2m)
    {
        Vec2f pos = _t2->getPos();
        pos.x += FlxG.elapsed*FlxG.width;
        if(pos.x > t2m) pos.x = t2m;
        _t2->setPos(pos);
    }

    //Check to see if the text is in position
    if(!_ok && ((_t1->x == t1m) || (_t2->x == t2m)))
    {
        //explosion
        _ok = true;
        // FlxG.play(SndHit);
        FlxG.flash.start(Color(0.85f, 0.92f, 0.64f),0.5);
        FlxG.quake.start(0.035,0.5);
        _t1->setColor(FlxU::color(0xd8eba2));
        _t2->setColor(FlxU::color(0xd8eba2));
        _gibs->start(true,5);
        _t1->angle = FlxU::random()*40-20;
        _t2->angle = FlxU::random()*40-20;

        TextPtr t2;
        ButtonPtr b;

        ResourceManager& res = *(FlxG.resources);
        TextPtr t1 = TextPtr(new Text(FlxG.width / 2 - 200,FlxG.height/3+39,400,"by Adam Atomic"));
        t1->setAlignment(ALIGN_CENTER);
        t1->setColor(FlxU::color(0x3a5c39));
        add(t1);

        //flixel button
        // add((new FlxSprite(t1m+1,FlxG.height/3+53)).createGraphic(106,19,0xff131c1b));

        b = ButtonPtr(new Button(t1m,FlxG.height/3+54,onFlixel));
        SpritePtr image(new Sprite());
        image->createGraphic(84, 15, FlxU::color(0xff3a5c39));
        SpritePtr imageHi(new Sprite());
        imageHi->createGraphic(84, 15,FlxU::color(0xff729954));
        b->loadGraphic(image, imageHi);
        t1 = TextPtr(new Text(1,3,84,"www.flixel.org"));
        t1->setColor(FlxU::color(0x729954));
        t1->setAlignment(ALIGN_CENTER);
        t2 = TextPtr(new Text(t1->x,t1->y,t1->width,t1->getText()));
        t2->setColor(FlxU::color(0xd8eba2));
        t2->setAlignment(t1->getAlignment());
        b->loadText(t1,t2);
        add(b);

    //    //danny B button
    //    // this.add((new FlxSprite(t1m+1,FlxG.height/3+75)).createGraphic(106,19,0xff131c1b));
    //    // b = new FlxButton(t1m+2,FlxG.height/3+76,onDanny);
    //    // b.loadGraphic((new FlxSprite()).createGraphic(104,15,0xff3a5c39),(new FlxSprite()).createGraphic(104,15,0xff729954));
    //    // t1 = new Text(8,1,100,"music by danny B");
    //    // t1.color = 0x729954;
    //    // t2 = new Text(t1.x,t1.y,t1.width,t1.text);
    //    // t2.color = 0xd8eba2;
    //    // b.loadText(t1,t2);
    //    // add(b);

        //play button
        _b = ButtonPtr(new Button(t1m,FlxG.height/3+100,onButton));
        image = SpritePtr(new Sprite());
        image->createGraphic(84, 30, FlxU::color(0xff3a5c39));
        imageHi = SpritePtr(new Sprite());
        imageHi->createGraphic(84, 30, FlxU::color(0xff729954));
        _b->loadGraphic(image, imageHi);
        t1 = TextPtr(new Text(0,5,84,"PLAY"));
        t1->setColor(FlxU::color(0x729954));
        t1->setSize(20.0f);
        t1->setAlignment(ALIGN_CENTER);
        t2 = TextPtr(new Text(t1->x,t1->y,t1->width,t1->getText()));
        t2->setColor(FlxU::color(0xd8eba2));
        t2->setSize(20.0f);
        t2->setAlignment(t1->getAlignment());
        _b->loadText(t1,t2);
        add(_b);
    }

    //X + C were pressed, fade out and change to play state
    // if(_ok && !_ok2 && FlxG.touch.justPressed())
    // {
    //     _ok2 = true;
    //     // FlxG.play(SndHit2);
    //     FlxG.flash.start(Color(0.85f, 0.92f, 0.64f),0.5);
    //     FlxG.fade.start(Color::black(),1,onFade);
    // }

    State::update();
}

