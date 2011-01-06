#pragma once

#include "flx/sprite.h"
#include "flx/phy/pworld.h"

#include "Box2D/Dynamics/b2Fixture.h"

namespace flx {
    
enum BodyType
{
    STATIC,
    DYNAMIC,
    KINEMATIC
};

/** Physics sprite */
class PSprite : public Sprite
{
public:
    PSprite(PWorldPtr world, BodyType bodyType, ci::Vec2f pos);
    virtual ~PSprite();

    void createFixture(b2FixtureDef& fixtureDef);
    void createFixture(b2Shape& shape, float density);

    void setSize(float width, float height);

    virtual void update();
    virtual void render();

    virtual void calcOrigin();

protected:
    b2World*   _world;
    b2Body*    _body;
    b2Fixture* _fixture;

private:
    void createBody(BodyType bodyType, ci::Vec2f pos);
};

typedef shared_ptr<PSprite> PSpritePtr;

}

