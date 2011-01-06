#include "flx/flx.h"
#include "flx/sprite.h"
#include "flx/emitter.h"
#include "flx/tileblock.h"
#include "flx/text.h"

#include "flx/samples/Play.h"

#include "bluegin/graphic.h"

using namespace flx;
using namespace cinder;
using namespace bluegin;

extern FlxGlobal FlxG;

class Player : public Sprite
{
public:
    Player(float x, float y, Graphic graphic) : Sprite(x, y)
    {
        loadGraphic(graphic, true);
        maxVelocity.x  = 100;
        acceleration.y = 400;
        drag.x = maxVelocity.x * 4;

        int walkFrames[] = { 1, 2, 3, 0 };
        addAnimation("walk", walkFrames, 4, 10, true);
    }

    virtual ~Player()
    {
        Log("~Player()");
    }

    void update()
    {
        acceleration.x = 0;
        // acceleration.x += 4 * drag.x;
        Sprite::update();
        play("walk");
        //Log("Player::update() (%f, %f)", x, y);
    }

    void render()
    {
        Sprite::render();
        //Log("Player::render()");
    }
};

class Elevator : public Sprite
{
protected:
    //  elevator initial y position
    float _y;
    //  travel height
    float _height;
public:
    Elevator(float x, float y, float h, Graphic graphic) 
        : Sprite(x, y, graphic)
    {
        width = 24;
        height = 10;
        _y = y;
        _height = h;
        fixed = true;
        velocity.y = 40;
    }

    virtual ~Elevator()
    {
        
    }

    void update()
    {
        Sprite::update();
        if (y > _y + _height) {
            y = _y + _height;
            velocity.y = -velocity.y;
        }
        else if (y < _y) {
            y = _y;
            velocity.y = -velocity.y;
        }
    }

    void render()
    {
        Sprite::render();
        //Log("Elevator::render(x %f y %f w %f h %f) origin(x %f y %f)", x, y, width, height,
        //        origin.x, origin.y);
        getScreenXY(_point);
    }
};

Play::Play() : State()
{
}

Play::~Play()
{
}

void Play::create()
{
    ResourceManager& res = *(FlxG.resources);

    shared_ptr<Text> ptext(new Text(0, 0, 100, "HELLO, world!"));
    Text& text = *ptext;
    text.setFont(res.font("font_a"));
    text.setColor(Color::black());
    text.setSize(12.0f);
    add(ptext);

    add(ObjectPtr(new Player(20, 50, res.graphic("player"))));
    add(ObjectPtr(new Elevator(20, 100, 40, res.graphic("gibs"))));

    shared_ptr<Emitter> pdisp(new Emitter(200, 40));
    Emitter& dispenser = *pdisp;
    dispenser.setSize(8.0f, 24.0f);
    dispenser.setXSpeed(-100, 200);
    dispenser.setYSpeed(-50, 50);
    dispenser.createSprites(res.graphic("gibs"), 150, 16, true, 0.8f);
    dispenser.start(false, 0.035f);
    add(pdisp);

    shared_ptr<Tileblock> ptb(new Tileblock(150, 100, 36, 12));
    Tileblock& tileblock = *ptb;
    tileblock.loadGraphic(res.graphic("gibs"), 0);
    add(ptb);

    //  set background color to yellow
    State::bgColor = ColorA(0, 0, 0, 1.0f);
    FlxG.fade.start(Color(1.0f, 0, 0), 2.0f);
    FlxG.quake.start(0.05f, 2.0f);
}

void Play::update()
{
    Log("Play::update()");
    float timer = 0;
    timer += FlxG.elapsed;

    State::update();
    collide();

    if (FlxG.fade.alpha == 1.0f) {
        if (FlxG.fade.exists) {
            FlxG.flash.start(Color(1.0f, 1.0f, 1.0f), 0.5f);
        }
        FlxG.quake.stop();
        FlxG.fade.stop();
    }
}

void Play::preProcess()
{
    gl::clear(bgColor);
}

void Play::postProcess()
{
}
