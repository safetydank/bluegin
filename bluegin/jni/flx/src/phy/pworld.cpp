#include "flx/flxG.h"
#include "flx/phy/pworld.h"

using namespace ci;
using namespace flx;

const int velocityIterations = 6;
const int positionIterations = 2;

extern FlxGlobal FlxG;

PWorld::PWorld(ci::Vec2f gravity, bool doSleep, float pixelScale)
{
    _world = new b2World(b2Vec2(gravity.x, gravity.y), doSleep);
    _pixelScale = pixelScale;

    //  Debug draw physics bodies
    _debugDraw = PDebugDrawPtr(new PDebugDraw(FlxG.camera));
    unsigned int flags = 0;
    flags |= b2DebugDraw::e_shapeBit |
             b2DebugDraw::e_jointBit |
             b2DebugDraw::e_aabbBit  |
             b2DebugDraw::e_pairBit  |
             b2DebugDraw::e_centerOfMassBit;
    _debugDraw->SetFlags(flags);

    _world->SetDebugDraw(_debugDraw.get());
}

PWorld::~PWorld()
{
    delete _world;
}

void PWorld::updatePhysics()
{
    const float timeStep = 1.0f / 60.0f;
    _world->Step(timeStep, velocityIterations, positionIterations);
    _world->ClearForces();
}

void PWorld::update()
{
    updatePhysics();
    updateMembers();
    updateFlickering();
}

void PWorld::render()
{
    _world->DrawDebugData();
    Group::render();
}

b2World* PWorld::getWorld()
{
    return _world;
}
