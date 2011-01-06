#include "flx/phy/psprite.h"

using namespace ci;
using namespace bluegin;
using namespace flx;

PSprite::PSprite(PWorldPtr world, BodyType bodyType, ci::Vec2f pos)
    : _world(world->getWorld())
{
    assert(_world);

    b2BodyDef bodyDef;
    switch (bodyType) {
    case DYNAMIC:
        bodyDef.type = b2_dynamicBody;
        break;
    case KINEMATIC:
        bodyDef.type = b2_kinematicBody;
        break;
    case STATIC:
    default:
        bodyDef.type = b2_staticBody;
        break;
    }
    bodyDef.position.Set(pos.x, pos.y);
    _body = _world->CreateBody(&bodyDef);
}

PSprite::~PSprite()
{
}

void PSprite::createFixture(b2FixtureDef& fixtureDef)
{
    _body->CreateFixture(&fixtureDef);
}

void PSprite::createFixture(b2Shape& shape, float density)
{
    _body->CreateFixture(&shape, density);
}

void PSprite::setSize(float width, float height)
{
    this->width  = width;
    this->height = height;
    resetHelpers();
}

void PSprite::update()
{
    //  XXX sync to physics xform
    b2Vec2 pos = _body->GetPosition();
    x = pos.x - origin.x; y = pos.y - origin.y;
    angle = toDegrees(_body->GetAngle());

    updateAnimation();
    updateFlickering();
}

void PSprite::render()
{
    // Sprite::render();
}

void PSprite::calcOrigin()
{
    origin.x = width / 2;
    origin.y = height / 2;
}
