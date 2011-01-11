#include "flx/flxG.h"
#include "mode/Player.h"
#include "mode/Bullet.h"
#include "mode/PlayState.h"

using namespace bluegin;
using namespace ci;
using namespace flx;

extern FlxGlobal FlxG;

Player::Player(int X, int Y, vector<ObjectPtr>& Bullets, EmitterPtr Gibs)
    : flx::Sprite(X, Y), _bullets(Bullets), _gibs(Gibs)
{
    ResourceManager& res = *(FlxG.resources);
    loadGraphic(res.graphic("spaceman"),true,true,8);
    // res.primeSound(res.sound("SndJump"));

    _restart = 0;

    //bounding box tweaks
    width = 7;
    height = 6;
    offset.x = 1;
    offset.y = 1;

    //basic player physics
    int runSpeed = 80;
    drag.x = runSpeed*8;
    acceleration.y = 420;
    _jumpPower = 200;
    maxVelocity.x = runSpeed;
    maxVelocity.y = _jumpPower;

    //animations
    int idleFrames[] = { 0 };
    addAnimation("idle", 1, idleFrames);
    int runFrames[] = { 1, 2, 3, 0 };
    addAnimation("run", 4, runFrames, 12);
    int jumpFrames[] = { 4 };
    addAnimation("jump", 1, jumpFrames);
    int idleUpFrames[] = { 5 };
    addAnimation("idle_up", 1, idleUpFrames);
    int runUpFrames[] = { 6, 7, 8, 5 };
    addAnimation("run_up", 4, runUpFrames, 12);
    int jumpUpFrames[] = { 9 };
    addAnimation("jump_up", 1, jumpUpFrames);
    int jumpDownFrames[] = { 10 };
    addAnimation("jump_down", 1, jumpDownFrames);

    //bullet stuff
    _bullets = Bullets;
    _curBullet = 0;
    _bulletVel = 360;
    _bulletDelay = 0;

    // jets
    _jets = Emitter::create();
    _jets->setRotation();
    _jets->gravity = 0;
    _jets->createSprites(res.graphic("jet"), 15, 0, false);
}

void Player::update()
{
    ResourceManager& res = *(FlxG.resources);

    //game restart timer
    if(dead)
    {
        _restart += FlxG.elapsed;
        if(_restart > 2) {
            static_cast<ReloadableState*>(FlxG.getState())->reload();
        }
        return;
    }

    //MOVEMENT
    acceleration.x = 0;

    if (FlxG.accel.y < -0.75f) {
        setFacing(LEFT);
        acceleration.x += drag.x * math<float>::clamp(FlxG.accel.y, -5.0f, -1.0f) * 0.5f;
    }
    else if (FlxG.accel.y > 0.75f) {
        setFacing(RIGHT);
        acceleration.x += drag.x * math<float>::clamp(FlxG.accel.y, 1.0f, 5.0f) * 0.5f;
    }

    // if(FlxG.keys.LEFT)
    // {
    //     facing = LEFT;
    //     acceleration.x -= drag.x;
    // }
    // else if(FlxG.keys.RIGHT)
    // {
    //     facing = RIGHT;
    //     acceleration.x += drag.x;
    // }
    // if(FlxG.keys.justPressed("X") && !velocity.y)
    // {
    //     velocity.y = -_jumpPower;
    //     FlxG.play(SndJump);
    // }

    // if(FlxG.touch.justPressed() && !velocity.y)
    // {
    //     velocity.y = -_jumpPower;
    //     FlxG.play(res.sound("SndJump"));
    // }
    if (FlxG.touch.pressed()) {
        velocity.y = -65.0f;
        _jets->at(*this);
        _jets->setXSpeed(-velocity.x*2.0f,-velocity.y*1.0f);
        _jets->setYSpeed(-velocity.y*2.0f, -velocity.y*1.0f);
        if (!_jets->on)
            _jets->start(false, 0.01f, 0);
    }
    else {
        _jets->stop(0.1f);
    }

    //AIMING
    _up = false;
    _down = false;
    // if(FlxG.keys.UP) _up = true;
    // else if(FlxG.keys.DOWN && velocity.y) _down = true;

    //ANIMATION
    if(velocity.y != 0)
    {
        if(_up) play("jump_up");
        else if(_down) play("jump_down");
        else play("jump");
    }
    else if(velocity.x == 0)
    {
        if(_up) play("idle_up");
        else play("idle");
    }
    else
    {
        if(_up) play("run_up");
        else play("run");
    }

    //SHOOTING
    if (_bulletDelay > 0) {
        _bulletDelay -= FlxG.elapsed;
    }
    if (_bulletDelay < 0) {
        _bulletDelay = 0;
    }

    if(!flickering() && _bulletDelay == 0)
    {
        int bXVel = 0;
        int bYVel = 0;
        int bX = x;
        int bY = y;

        Bullet& bullet = static_cast<Bullet&>(*(_bullets[_curBullet]));
        if(getFacing() == RIGHT)
        {
            bX += width - 4;
            bXVel = _bulletVel;
        }
        else
        {
            bX -= bullet.width - 4;
            bXVel = -_bulletVel;
        }
        bullet.shoot(bX,bY,bXVel,bYVel);
        _bulletDelay = 0.2f;
        if(++_curBullet >= _bullets.size())
            _curBullet = 0;

    //     if(_up)
    //     {
    //         bY -= _bullets[_curBullet].height - 4;
    //         bYVel = -_bulletVel;
    //     }
    //     else if(_down)
    //     {
    //         bY += height - 4;
    //         bYVel = _bulletVel;
    //         velocity.y -= 36;
    //     }
    //     else if(facing == RIGHT)
    //     {
    //         bX += width - 4;
    //         bXVel = _bulletVel;
    //     }
    //     else
    //     {
    //         bX -= _bullets[_curBullet].width - 4;
    //         bXVel = -_bulletVel;
    //     }
    //     _bullets[_curBullet].shoot(bX,bY,bXVel,bYVel);
    //     if(++_curBullet >= _bullets.length)
    //         _curBullet = 0;
    }

    //UPDATE POSITION AND ANIMATION
    _jets->update();
    Sprite::update();

    //Jammed, can't fire!
    // if(flickering())
    // {
    //     if(FlxG.keys.justPressed("C"))
    //         FlxG.play(SndJam);
    // }
}

void Player::render()
{
    _jets->render();
    Sprite::render();
}

void Player::hitLeft(flx::Object& Contact, float Velocity)
{
    Sprite::hitLeft(Contact,Velocity);
}

void Player::hitRight(flx::Object& Contact, float Velocity)
{
    Sprite::hitRight(Contact,Velocity);
}

void Player::hitTop(flx::Object& Contact, float Velocity)
{
    Sprite::hitTop(Contact,Velocity);
}

void Player::hitBottom(Object& Contact, float Velocity)
{
    // if(velocity.y > 50)
    //     FlxG.play(SndLand);
    onFloor = true;
    Sprite::hitBottom(Contact,Velocity);
}

void Player::hurt(float Damage)
{
    Damage = 0;
    if(flickering())
        return;
    // FlxG.play(SndHurt);
    flicker(1.3);
    if(FlxG.score > 1000) FlxG.score -= 1000;
    if(velocity.x > 0)
        velocity.x = -maxVelocity.x;
    else
        velocity.x = maxVelocity.x;
    Sprite::hurt(Damage);
}


void Player::kill()
{
    ResourceManager& res = *(FlxG.resources);

    if(dead)
        return;
    solid = false;
    FlxG.play(res.sound("SndExplode"));
    FlxG.play(res.sound("SndExplode2"));
    Sprite::kill();
    flicker(-1);
    exists = true;
    visible = false;
    FlxG.quake.start(0.005, 0.35);
    FlxG.flash.start(Color(0.85f, 0.92f, 0.64f), 0.35);
    if(_gibs)
    {
        _gibs->at(*this);
        _gibs->start(true,0,50);
    }
}

