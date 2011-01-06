#include "bluegin/state.h"

using namespace bluegin;

void State::enter()
{
    this->onEnter();
}

void State::leave()
{
    this->onLeave();
}

void State::update(float dt)
{
    this->onUpdate(dt);
}

void State::draw()
{
    this->onDraw();
}

