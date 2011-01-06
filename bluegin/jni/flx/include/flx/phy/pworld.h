#pragma once

#include "flx/group.h"
#include "flx/phy/pdebugdraw.h"
#include "Box2D/Dynamics/b2World.h"

namespace flx {

/** Physics world */
class PWorld : public flx::Group
{
public:
    PWorld(ci::Vec2f gravity, bool doSleep=true, float pixelScale=30.0f);
    virtual ~PWorld();

    virtual void update();
    virtual void render();

    void updatePhysics();

    b2World* getWorld();

protected:
    b2World* _world;
    float _pixelScale;

    PDebugDrawPtr _debugDraw;
};

typedef shared_ptr<PWorld> PWorldPtr;

}
