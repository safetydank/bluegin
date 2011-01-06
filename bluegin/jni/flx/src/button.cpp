#include "flx/button.h"
#include "flx/flxG.h"

using namespace ci;
using namespace flx;

extern FlxGlobal FlxG;

Button::Button(int X,int Y,ButtonCallback Callback)
    : Group()
{
    x = X;
    y = Y;
    width = 100;
    height = 20;
    _off = SpritePtr(new Sprite());
    _off->createGraphic(width,height,FlxU::color(0xff7f7f7f));
    _off->solid = false;
    add(_off,true);
    _on  = SpritePtr(new Sprite());
    _on->createGraphic(width,height,FlxU::color(0xffffffff));
    _on->solid = false;
    add(_on,true);
    _offT = TextPtr();
    _onT = TextPtr();
    _callback = Callback;
    _onToggle = false;
    _pressed = false;
    _initialized = false;
    _sf.set(0, 0);
}
    
Button::~Button()
{
    FlxG.touch.removeTouchListener(this);
}

void Button::loadGraphic(SpritePtr Image,SpritePtr ImageHighlight)
{
    replace(_off,Image);
    _off = Image;
    if(!ImageHighlight)
    {
        if(_on != _off && bool(_on))
            remove(*_on);
        _on = _off;
    }
    else {
        replace(_on,ImageHighlight);
        _on = ImageHighlight;
    }

    Sprite& sOn = static_cast<Sprite&>(*_on);
    Sprite& sOff = static_cast<Sprite&>(*_off);
    sOn.solid = sOff.solid = false;
    sOff.scrollFactor = scrollFactor;
    sOn.scrollFactor = scrollFactor;
    width = sOff.width;
    height = sOff.height;
    refreshHulls();
}

void Button::loadText(TextPtr Text,TextPtr TextHighlight)
{
    if(Text)
    {
        if(!_offT)
        {
            _offT = Text;
            add(_offT);
        }
        else {
            replace(_offT,Text);
            _offT = Text;
        }
    }
    if(!TextHighlight)
        _onT = _offT;
    else
    {
        if(!_onT)
        {
            _onT = TextHighlight;
            add(_onT);
        }
        else {
            replace(_onT,TextHighlight);
            _onT = TextHighlight;
        }
    }

    flx::Text& tOff = static_cast<flx::Text&>(*_offT);
    flx::Text& tOn  = static_cast<flx::Text&>(*_onT);

    tOff.scrollFactor = scrollFactor;
    tOn.scrollFactor  = scrollFactor;
}
    
/**
 * Called by the game loop automatically, handles mouseover and click detection.
 */
void Button::update()
{
    if(!_initialized)
    {
        FlxG.touch.addTouchListener(this);
        _initialized = true;
    }
    
    Group::update();

    if(overlapsPoint(Vec2f(FlxG.touch.x, FlxG.touch.y))) {
        _pressed = FlxG.touch.justPressed() || (_pressed && FlxG.touch.pressed());
    }
    else {
        _pressed = false;
    }
    visibility(_pressed);

    if(_onToggle) visibility(_off->visible);
}
    
/**
 * Use this to toggle checkbox-style behavior.
 */
bool Button::getOn()
{
    return _onToggle;
}

/**
 * @private
 */
void Button::setOn(bool On)
{
    _onToggle = On;
}

/**
 * Internal function for handling the visibility of the off and on graphics.
 * 
 * @param	On		Whether the button should be on or off.
 */
void Button::visibility(bool On)
{
    if(On)
    {
        _off->visible = false;
        if(_offT) _offT->visible = false;
        _on->visible = true;
        if(_onT) _onT->visible = true;
    }
    else
    {
        _on->visible = false;
        if(_onT) _onT->visible = false;
        _off->visible = true;
        if(_offT) _offT->visible = true;
    }
}
    
void Button::handleTouchUp(float X, float Y, float XScroll, float YScroll)
{
    if(!exists || !visible || !active || !FlxG.touch.justReleased() || (_callback == NULL)) return;
    if(_pressed && overlapsPoint(Vec2f(FlxG.touch.x,FlxG.touch.y))) _callback(NULL);
}

