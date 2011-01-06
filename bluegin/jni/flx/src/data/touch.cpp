#include "flx/flx.h"
#include "flx/data/touch.h"

#include <algorithm>

using namespace flx;

Touch::Touch()
{
    x = 0;
    y = 0;
    screenX = 0;
    screenY = 0;
    _current = 0;
    _last = 0;
}

void Touch::update()
{
    if((_last == -1) && (_current == -1))
        _current = 0;
    else if((_last == 2) && (_current == 2))
        _current = 1;
    _last = _current;
}

void Touch::reset()
{
    _current = 0;
    _last = 0;
}

bool Touch::pressed()
{
    return (_current > 0);
}

bool Touch::justPressed()
{
    return (_current == 2);
}

bool Touch::justReleased()
{
    return (_current == -1);
}

void Touch::handleTouchDown(float X, float Y, float XScroll, float YScroll)
{
    if (_current > 0) _current = 1;
    else _current = 2;
    handleTouchMoved(X, Y, XScroll, YScroll);
    for (vector<TouchListener*>::iterator listener = _listeners.begin();
         listener != _listeners.end(); ++listener) {
        (*listener)->handleTouchDown(X, Y, XScroll, YScroll);
    }
}

void Touch::handleTouchMoved(float X, float Y, float XScroll, float YScroll)
{
    screenX = X;
    screenY = Y;
    x = screenX - FlxU::floor(XScroll);
    y = screenY - FlxU::floor(YScroll);
    // Log("handleTouchMoved: x %f y %f", X, Y);
    for (vector<TouchListener*>::iterator listener = _listeners.begin();
         listener != _listeners.end(); ++listener) {
        (*listener)->handleTouchMoved(X, Y, XScroll, YScroll);
    }
}

void Touch::handleTouchUp(float X, float Y, float XScroll, float YScroll)
{
    if (_current > 0) _current = -1;
    else _current = 0;
    handleTouchMoved(X, Y, XScroll, YScroll);
    for (vector<TouchListener*>::iterator listener = _listeners.begin();
         listener != _listeners.end(); ++listener) {
        (*listener)->handleTouchUp(X, Y, XScroll, YScroll);
    }
}

void Touch::addTouchListener(TouchListener* listener)
{
    _listeners.push_back(listener);
}

void Touch::removeTouchListener(TouchListener* listener)
{
    vector<TouchListener*>::iterator it = std::find(_listeners.begin(), _listeners.end(), listener);
    if (it != _listeners.end()) {
        _listeners.erase(it);
    }
}
