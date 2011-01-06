#include "flx/sprite.h"

class Bullet : public flx::Sprite
{
public:
    Bullet();

    virtual void update();
    virtual void render();
    virtual void hitLeft(Object& Contact, float Velocity);
    virtual void hitRight(Object& Contact, float Velocity);
    virtual void hitBottom(Object& Contact, float Velocity);
    virtual void hitTop(Object& Contact, float Velocity);
    virtual void kill();
    void shoot(int X, int Y, int VelocityX, int VelocityY);
};

