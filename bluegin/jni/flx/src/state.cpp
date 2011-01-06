#include "flx/state.h"
#include "cinder/gl/gl.h"

using namespace flx;
using namespace ci;

Color State::bgColor;

State::State()
{
}

State::~State()
{
}

void State::create()
{
}

void State::update()
{
    defaultGroup.update();
}

void State::collide()
{
    defaultGroup.collide(defaultGroup);
}

void State::preProcess()
{
    gl::clear(bgColor);
}

void State::render()
{
    defaultGroup.render();
}

void State::destroy()
{
    defaultGroup.destroy();
}

void State::add(ObjectPtr core)
{
    defaultGroup.add(core);
}

void State::postProcess()
{
}

