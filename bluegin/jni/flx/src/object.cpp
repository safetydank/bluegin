#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/object.h"
#include "cinder/Rect.h"

using namespace cinder;
using namespace flx;

extern FlxGlobal FlxG;

Object::Object(float x, float y, float w, float h)
    : Rect(x, y, w, h), _kind(OBJECT)
{
    exists = true;
    active = true;
    visible = true;
    solid = true;
    fixed = false;
    moves = true;

    collideLeft = true;
    collideRight = true;
    collideTop = true;
    collideBottom = true;

    origin.set(0, 0);

    velocity.set(0, 0);
    acceleration.set(0, 0);
    drag.set(0, 0);
    maxVelocity.set(10000,10000);

    // angle in degrees
    angle = 0;
    angularVelocity = 0;
    angularAcceleration = 0;
    angularDrag = 0;
    maxAngular = 10000;

    thrust = 0;
    maxThrust = 0;

    scrollFactor.set(1.0, 1.0);
    _flicker = false;
    _flickerTimer = -1.0f;
    health = 1;
    dead = false;

    _rect.set(0,0,0,0);

    colHullX.set(0,0,0,0);
    colHullY.set(0,0,0,0);
    colVector.set(0, 0);
    colOffsets.push_back(Vec2f(0,0));
    _group = false;

    kind(0);
}

Object::~Object()
{
}

void Object::refreshHulls()
{
   colHullX.set(x, y, width, height); 
   colHullY.set(x, y, width, height);
}

void Object::updateMotion()
{
    if (!moves)
        return;

    if (solid)
        refreshHulls();

    onFloor = false;

    float vc = (FlxU::computeVelocity(angularVelocity,angularAcceleration,angularDrag,maxAngular) - angularVelocity)/2;
    angularVelocity += vc; 
    angle += angularVelocity*FlxG.elapsed;
    angularVelocity += vc;

    Vec2f thrustComponents(0,0);
    if (thrust != 0) {
        thrustComponents    = FlxU::rotatePoint(-thrust,0,0,0,angle);
        Vec2f maxComponents = FlxU::rotatePoint(-maxThrust,0,0,0,angle);
        float max = ((maxComponents.x>0)?maxComponents.x:-maxComponents.x);
        if (max > ((maxComponents.y>0)?maxComponents.y:-maxComponents.y))
            maxComponents.y = max;
        else
            max = ((maxComponents.y>0)?maxComponents.y:-maxComponents.y);
        maxVelocity.x = maxVelocity.y = ((max>0)?max:-max);
    }
    else {
        thrustComponents = Vec2f(0, 0);
    }

    // velocity.x = FlxU::computeVelocity(velocity.x, acceleration.x+thrustComponents.x,
    //         drag.x, maxVelocity.x);
    // velocity.y = FlxU::computeVelocity(velocity.y, acceleration.y+thrustComponents.y,
    //         drag.y, maxVelocity.y);

    vc = (FlxU::computeVelocity(velocity.x,acceleration.x+thrustComponents.x,drag.x,maxVelocity.x) - velocity.x)/2;
    velocity.x += vc;
    float xd = velocity.x * FlxG.elapsed;
    velocity.x += vc;

    vc = (FlxU::computeVelocity(velocity.y,acceleration.y+thrustComponents.y,drag.y,maxVelocity.y) - velocity.y)/2;
    velocity.y += vc;
    float yd = velocity.y * FlxG.elapsed;
    velocity.y += vc;

    x += xd;
    y += yd;

    // Update collision data with new movement results
    if (!solid)
        return;

    colVector.set(xd, yd);
    colHullX.width += ((colVector.x>0)?colVector.x:-colVector.x);
    if (colVector.x < 0) 
        colHullX.x += colVector.x;
    colHullY.x = x;
    colHullY.height += ((colVector.y>0)?colVector.y:-colVector.y);
    if(colVector.y < 0)
        colHullY.y += colVector.y;
}

void Object::updateFlickering()
{
    if(flickering())
    {
        if(_flickerTimer > 0)
        {
            _flickerTimer -= FlxG.elapsed;
            if(_flickerTimer == 0)
                _flickerTimer = -1;
        }
        if(_flickerTimer < 0)
            flicker(-1);
        else
        {
            _flicker = !_flicker;
            visible = !_flicker;
        }
    }
}

void Object::update()
{
    updateMotion();
    updateFlickering();
}

bool Object::overlaps(Object& object)
{
    getScreenXY(_point);
    float tw = hasFrame() ? getFrameWidth()  : width;
    float th = hasFrame() ? getFrameHeight() : height;
    Rectf t(_point.x, _point.y, _point.x + tw, _point.y + th);

    object.getScreenXY(_point);
    float ow = object.hasFrame() ? object.getFrameWidth()  : width;
    float oh = object.hasFrame() ? object.getFrameHeight() : height;
    Rectf o(_point.x, _point.y, _point.x + ow, _point.y + oh);
    return t.intersects(o);
}

bool Object::overlapsPoint(Vec2f pt)
{
    pt.x += FlxG.scroll.x;
    pt.y += FlxG.scroll.y;
    getScreenXY(_point);
    if ( (pt.x <= _point.x) || (pt.x >= _point.x+width) 
            || (pt.y <= _point.y) || (pt.y >= _point.y+height))
        return false;
    return true;
}

bool Object::collide(Object& object)
{
    return FlxU::collide(*this, object);
}

void Object::hitLeft(Object& contact, float velx)
{
    if (!fixed)
        velocity.x = velx;
}

void Object::hitRight(Object& contact, float velx)
{
    hitLeft(contact, velx);
}

void Object::hitTop(Object& contact, float vely)
{
    if (!fixed)
        velocity.y = vely;
}

void Object::hitBottom(Object& contact, float vely)
{
    onFloor = true;
    if (!fixed)
        velocity.y = vely;
}

void Object::hurt(int damage)
{
    health -= damage;
    if (health <= 0) kill();
}

void Object::kill()
{
    exists = false;
    dead = true;
}

void Object::flicker(float Duration)
{
    _flickerTimer = Duration; 
    if(_flickerTimer < 0) { 
        _flicker = false; 
        visible = true; 
    }
}

bool Object::flickering()
{
    return _flickerTimer >= 0;
}

Vec2d& Object::getScreenXY(Vec2d& pt)
{
    // XXX removed floor call on x/y
    pt.x = x + FlxG.scroll.x * scrollFactor.x;
    pt.y = y + FlxG.scroll.y * scrollFactor.y;
    return pt;
}

bool Object::onScreen()
{
    getScreenXY(_point);

    if ((_point.x + width < 0) || (_point.x > FlxG.width) 
            || (_point.y + height < 0) || (_point.y > FlxG.height))
        return false;
    return true;
}

void Object::reset(float x, float y)
{
    this->x = x;
    this->y = y;
    exists = true;
    dead = false;
}

int Object::kind(int setKind)
{
    if (setKind >= 0)
        _kind = setKind;
    return _kind;
}


